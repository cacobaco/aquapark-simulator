# AquaPark Simulator

AquaPark Simulator is a **C-based simulation of a water park**, focusing on **solving synchronization problems**. Visitors can join attraction queues, leave queues, gain priority access, and more. This project was developed as part of the **Operating Systems** course during the **2023/2024 academic year**, in the third and final year of the **Bachelor's Degree in Computer Engineering**.

## Project Overview
This project consists of two programs:
- **Simulator**: Runs the simulation in the background, managing visitor interactions and queue dynamics.
- **Monitor**: Provides a user interface to start the simulation and visualize real-time events happening in the water park.

A connection is established between the **Simulator** and **Monitor** to display simulation events to the user.

## Compilation
Run the following command to compile the project:
```bash
make
```

## Configuration
The simulation parameters can be configured in the `config.json` file. The available settings include:
- **Probabilities** – Adjust the likelihood of visitors entering/exiting queues.
- **Timing Settings** – Define time intervals for visitor actions.
- **Attractions** – Customize the available park attractions.

## Execution
Start the programs in the following order:
```bash
./monitor
./simulador
```

## Team
- **Leonardo Fernandes** (me)
- **João Miguel**

