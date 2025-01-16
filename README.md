# Deribit Order Execution and Management System (C++)

## Overview
This project aims to develop a high-performance order execution and management system for trading on Deribit Test (https://test.deribit.com/) using C++. The system will enable fast and reliable trading by handling order placement, modification, cancellation, and real-time market data streaming.

## Setup Guide

### Initial Setup
1. **Create a Deribit Test account**: Register for a new account at [Deribit Test](https://test.deribit.com/).
2. **Generate API Keys**: Generate API keys from the Deribit Test platform for authentication.

### Core Features
- **Order Management Functions**:
  - Place new orders
  - Cancel existing orders
  - Modify orders
  - Fetch the orderbook
  - View current open positions
  
- **Real-time Market Data Streaming**:
  - Implement WebSocket server functionality (currently under process)
  - Allow clients to subscribe to symbols
  - Stream continuous updates of the orderbook for subscribed symbols

- **Market Coverage**:
  - Supported instruments: Spot, Futures, and Options
  - Covers all supported trading pairs and symbols on Deribit

### Technical Requirements
- **Language**: C++ implementation for optimal performance.
- **Low-latency**: The system is designed to minimize latency for order execution and data retrieval.
- **Error Handling & Logging**: Proper error management and logging functionality are implemented for debugging and monitoring.
- **WebSocket Integration**: A WebSocket server is under development to enable real-time data distribution, which will include:
  - Connection management
  - Subscription handling
  - Efficient message broadcasting

## Dependencies
- **vcpkg**: A C++ package manager to manage libraries and dependencies.
- **nlohmann/json**: A modern C++ library for JSON parsing.
- **curl**: Used for making HTTP requests for API interaction.

## Windows Setup Procedure

### 1. Install Dependencies via vcpkg
Ensure you have **vcpkg** installed and integrated into your C++ environment.

- **Install vcpkg**:
  - Download from [vcpkg GitHub](https://github.com/microsoft/vcpkg).

### 2. Clone the Repository
Clone the repository to your local machine:

```bash
git clone <repository_url>
cd <repository_name>
```

### 3. Configure CMake with vcpkg Toolchain
Configure your project to use the vcpkg toolchain so that dependencies are correctly linked:

```bash
cmake -DCMAKE_TOOLCHAIN_FILE=<path_to_vcpkg>/scripts/buildsystems/vcpkg.cmake ..
```
### 4. Build the Project
After setting up dependencies, build the project:

```bash
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=<path_to_vcpkg>/scripts/buildsystems/vcpkg.cmake ..
cmake --build . --config Release
```
### 5. Running the Application
Once the build is complete, you can run the executable:

```bash
./DeribitOrderSystem
```


