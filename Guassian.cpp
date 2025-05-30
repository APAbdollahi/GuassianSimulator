#include <iostream>
#include <random>
#include <vector>
#include <numeric>
#include <fstream>
#include <stdexcept>
#include <iomanip>

class TradingStrategy {
private:
    // Random number generator for price simulation
    mutable std::mt19937 generator_;
    
public:
    TradingStrategy() : generator_(std::random_device{}()) {}
    
    // Calculate simple moving average for a given window
    double calculateMovingAverage(const std::vector<double>& series, 
                                 int endIndex, 
                                 int windowSize) const {
        if (endIndex < windowSize - 1 || endIndex >= static_cast<int>(series.size())) {
            throw std::out_of_range("Invalid index or insufficient data for moving average");
        }
        
        double sum = 0.0;
        for (int i = 0; i < windowSize; ++i) {
            sum += series[endIndex - i];
        }
        return sum / windowSize;
    }
    
    // Generate trading signal based on moving average crossover
    // Returns: 1 for buy, -1 for sell, 0 for no signal
    int generateMACrossSignal(const std::vector<double>& prices, 
                             int shortWindow, 
                             int longWindow, 
                             int currentIndex) const {
        if (currentIndex < longWindow - 1) {
            return 0; // Not enough data
        }
        
        double shortMA = calculateMovingAverage(prices, currentIndex, shortWindow);
        double longMA = calculateMovingAverage(prices, currentIndex, longWindow);
        
        if (shortMA > longMA) {
            return 1;  // Buy signal
        } else if (shortMA < longMA) {
            return -1; // Sell signal
        }
        return 0; // No clear signal
    }
    
    // Calculate P&L for the moving average crossover strategy
    double calculateStrategyPnL(const std::vector<double>& prices, 
                               int shortWindow, 
                               int longWindow, 
                               int step = 1) const {
        if (prices.size() < static_cast<size_t>(longWindow)) {
            return 0.0; // Not enough data
        }
        
        std::vector<double> entryPrices;
        std::vector<int> signals;
        double totalPnL = 0.0;
        int currentSignal = 0;
        
        // Generate signals and track entry points
        for (int i = longWindow; i < static_cast<int>(prices.size()); i += step) {
            int newSignal = generateMACrossSignal(prices, shortWindow, longWindow, i);
            
            // Only act on signal changes
            if (newSignal != currentSignal && newSignal != 0) {
                entryPrices.push_back(prices[i]);
                signals.push_back(newSignal);
                currentSignal = newSignal;
            }
        }
        
        // Calculate P&L from trades
        for (size_t i = 1; i < entryPrices.size(); ++i) {
            double priceDiff = entryPrices[i] - entryPrices[i-1];
            
            // If previous signal was buy (1), we profit from price increase
            // If previous signal was sell (-1), we profit from price decrease
            if (signals[i-1] == 1) {
                totalPnL += priceDiff;  // Long position profits from price increase
            } else if (signals[i-1] == -1) {
                totalPnL -= priceDiff;  // Short position profits from price decrease
            }
        }
        
        return totalPnL;
    }
    
    // Generate simulated price series using Geometric Brownian Motion
    std::vector<double> generateSimulatedPrices(double drift, 
                                               double volatility, 
                                               double initialPrice, 
                                               int numSteps) const {
        std::vector<double> prices;
        prices.reserve(numSteps);
        
        std::normal_distribution<double> distribution(drift, volatility);
        double currentPrice = initialPrice;
        
        for (int i = 0; i < numSteps; ++i) {
            currentPrice *= (1.0 + distribution(generator_));
            prices.push_back(currentPrice);
        }
        
        return prices;
    }
    
    // Calculate average of a vector
    double calculateAverage(const std::vector<double>& values) const {
        if (values.empty()) {
            return 0.0;
        }
        
        double sum = std::accumulate(values.begin(), values.end(), 0.0);
        return sum / static_cast<double>(values.size());
    }
    
    // Run Monte Carlo simulation of the trading strategy
    void runSimulation(int numSimulations = 1000, 
                      int priceSeriesLength = 10000,
                      int shortWindow = 50, 
                      int longWindow = 150,
                      const std::string& outputFile = "trading_results.csv") const {
        
        std::vector<double> pnlResults;
        pnlResults.reserve(numSimulations);
        
        std::ofstream file(outputFile);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open output file: " + outputFile);
        }
        
        file << "simulation,pnl" << std::endl;
        
        std::cout << "Running " << numSimulations << " simulations..." << std::endl;
        std::cout << "Short MA: " << shortWindow << ", Long MA: " << longWindow << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        for (int i = 0; i < numSimulations; ++i) {
            // Generate price series (no drift, 1% volatility, starting at $100)
            auto prices = generateSimulatedPrices(0.0, 0.01, 100.0, priceSeriesLength);
            
            // Calculate P&L for this simulation
            double pnl = calculateStrategyPnL(prices, shortWindow, longWindow, 5);
            pnlResults.push_back(pnl);
            
            // Write to file
            file << i + 1 << "," << std::fixed << std::setprecision(4) << pnl << std::endl;
            
            // Progress update
            if ((i + 1) % 100 == 0) {
                std::cout << "Completed " << i + 1 << " simulations" << std::endl;
            }
        }
        
        file.close();
        
        // Calculate and display statistics
        double avgPnL = calculateAverage(pnlResults);
        double minPnL = *std::min_element(pnlResults.begin(), pnlResults.end());
        double maxPnL = *std::max_element(pnlResults.begin(), pnlResults.end());
        
        // Count profitable simulations
        int profitableCount = std::count_if(pnlResults.begin(), pnlResults.end(), 
                                          [](double pnl) { return pnl > 0; });
        
        std::cout << std::string(50, '-') << std::endl;
        std::cout << "Simulation Results:" << std::endl;
        std::cout << "Average P&L: $" << std::fixed << std::setprecision(2) << avgPnL << std::endl;
        std::cout << "Min P&L: $" << minPnL << std::endl;
        std::cout << "Max P&L: $" << maxPnL << std::endl;
        std::cout << "Profitable simulations: " << profitableCount << "/" << numSimulations 
                  << " (" << std::fixed << std::setprecision(1) 
                  << (100.0 * profitableCount / numSimulations) << "%)" << std::endl;
        std::cout << "Results saved to: " << outputFile << std::endl;
    }
};

int main() {
    try {
        TradingStrategy strategy;
        
        // Run simulation with default parameters
        strategy.runSimulation(1000, 10000, 50, 150, "ma_crossover_results.csv");
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
