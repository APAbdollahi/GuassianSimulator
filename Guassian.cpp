#include <iostream>
#include <random>
#include <vector>
#include <numeric>
#include <fstream>

// funtion to calculate average of a vector
double average(std::vector<double> const& v){
    if(v.empty()){
        return 0;
    }

    auto const count = static_cast<float>(v.size());
    return std::reduce(v.begin(), v.end()) / count;
}

//Sample signal generator returning +1 or -1 based on Moving Average Cross-over strategy as an example
int MA_Cross(std::vector<double> const& Series, int const& short_window,int const& long_window, int const& start){

//a short and a long time window 
double MA_short=10;
double MA_long=50;

for (int i=0; i<= short_window; ++i){
  MA_short += Series[(start-i)];
  //std::cout << Series[start-i]<<std::endl;
}
MA_short /= short_window;



for (int j=0; j<= long_window; ++j){
  MA_long += Series[(start-j)];
}
MA_long /= long_window;

if (MA_short> MA_long){return 1;}
else if (MA_short< MA_long){return -1;}
else {return 0;}
}

//PnL calculator fdr the MACross strategy
double MACross_Strat_PnL(std::vector<double> const& price, int const& short_window,int const& long_window, int step){
std::vector<double> price_hist;
std::vector<int> sig_hist;
double pnl=0;
int sig=0;

for (int j=long_window+1;j< price.size(); j += step){
    int generated_sig=MA_Cross(price, short_window, long_window,j);
     if (sig!=generated_sig){
       price_hist.push_back(price[j]);
       sig_hist.push_back(generated_sig);
       sig=generated_sig;
       }
  };
    
for (int k=1; k< price_hist.size(); ++k){
  if(sig_hist[k]==-1 && sig_hist[k-1]==1){
    pnl += price_hist[k]-price_hist[k-1];
  } else { pnl += price_hist[k-1]-price_hist[k];};};
  return pnl;
}

//Generting simulated prices based on Geometric Brownian Motion with no drift  
std::vector<double> Generate_Simulated_Price(double mu, double sigma, double S, int size){
  std::vector<double> Series;
  std::random_device rd {};
  std::mt19937 gen {rd()};
  std::normal_distribution<> d{mu, sigma};


  for (int i=0; i<size; ++i){
       S=S*(1+d(gen));
       Series.push_back(S);
      //std::cout << S << std::endl;

  };
return Series;

}


int main() {
  // initialize Gaussian random number generator
  std::vector<double> Simulated_price;
  std::vector<double> pnl_series;
  double pnl;
  std::ofstream myfile;
  myfile.open("output.csv");
  myfile << "pnl" << std::endl;
  for (int i=1; i< 10000; ++i){
    Simulated_price=Generate_Simulated_Price(0,0.01,100,100000);
    pnl=MACross_Strat_PnL(Simulated_price,50,150,5);
    pnl_series.push_back(pnl);
    std::cout << "pnl is" << pnl << std::endl;
    myfile << pnl << std::endl;

  }

std::cout << "average pnl is: "<< average(pnl_series)<<std::endl;
myfile.close();
  return 0;
}
