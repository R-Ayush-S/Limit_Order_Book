# Limit_Order_Book

# C++ Limit Order Book & Matching Engine

A high-performance C++ simulation of a financial exchange **Limit Order Book** with support for:

- BUY / SELL limit orders
- Market order matching
- Order cancellation
- Price-time priority
- Randomized stress simulation
- Internal consistency validation

This project demonstrates core concepts used in real trading systems and quantitative infrastructure.

---

## Features

- Separate BUY and SELL books using ordered maps
- FIFO matching within each price level
- Market and limit order handling
- Partial fills and multi-level matching
- Large-scale randomized simulation(Monte Carlo Simulation)
- Post-simulation integrity validation
- Performance timing
- On Console Logging(in main2.cpp)

---

## Accomplishment

- It takes 3sec to completely 100,000 trasactions without any kind of logging.
- It takes 10sec to complete 100,000 transactions with on Console Logging.


---
## Order Book Design

- BUY side: `map<price, list<order>, greater<>>` (highest price first)
- SELL side: `map<price, list<order>>` (lowest price first)
- Cancellation map:  
  `unordered_map<order_id, (price, side)>`

Matching follows **price–time priority**.

---

## Build & Run

### Requirements

- C++17 or later
- g++

---

### Compile

```bash
g++ -O2 -std=c++17 main.cpp -o orderbook
