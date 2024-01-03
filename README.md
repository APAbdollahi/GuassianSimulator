
# Financial Strategy Simulator

## Overview

This program is a C++ application designed to simulate financial trading strategies using Geometric Brownian Motion for price generation and Moving Average Cross-over strategy for trading signals. It calculates the Profit and Loss (PnL) of the strategy over simulated price data and outputs the results.

## Features

- **Geometric Brownian Motion**: Simulates price series based on a stochastic process.
- **Moving Average Cross-over Strategy**: Generates trading signals based on short-term and long-term moving averages.
- **PnL Calculation**: Computes the profitability of the trading strategy.
- **File Output**: Writes the PnL data to a CSV file for further analysis.

## Prerequisites

- C++17 compatible compiler (e.g., GCC, Clang, MSVC)
- Basic knowledge of C++ and command-line operations

## Installation

1. Clone the repository or download the source code.
2. Ensure you have a C++17 compatible compiler installed on your system.

## Compilation

Navigate to the directory containing the source code and compile using a C++ compiler. For example, using g++:

```bash
g++ -o finance_simulator main.cpp -std=c++17
```

## Usage

Run the compiled program from the command line:

```bash
./finance_simulator
```

The program will simulate price series, calculate PnL for each series, and write the results to `output.csv`. You can view the average PnL in the console output.

## Configuration

To modify the simulation parameters, such as the size of the time windows for the moving averages, edit the relevant variables in the `main` function.

## Output

The program generates a file named `output.csv` in the current directory, containing the PnL values for each simulation iteration.

## Contributing

Feel free to fork the repository and submit pull requests. For major changes, please open an issue first to discuss what you would like to change.

## License

This project is open-sourced under the MIT License. See the LICENSE file for more details.

## Disclaimer

This software is for educational and simulation purposes only. It should not be used for real financial trading without proper risk assessment and consultation with financial experts.
