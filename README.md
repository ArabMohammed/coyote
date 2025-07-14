# Coyote Environment Setup and Benchmark Guide

## Environment Installation Guide

Follow these steps to set up the project environment and dependencies.

### 1. Clone the Project Repository
```bash
cd /scratch/<your_user_id>/
git clone https://github.com/ArabMohammed/coyote.git
cd coyote
```
📌 This directory contains the `environment.yml` file used to configure the Conda environment.

### 2. Create and Activate the Conda Environment
```bash
conda env create -f environment.yml -n coyoteEnv
conda activate coyoteEnv
```

---

### 3. Install Microsoft SEAL (from Source)

```bash
cd /scratch/<your_user_id>/
git clone https://github.com/microsoft/SEAL.git
cd SEAL
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=$CONDA_PREFIX
cmake --build build
cmake --install build
```

This installs the Microsoft SEAL library into your active Conda environment.

---

### ✅ Setup Complete!
You are now ready to run the benchmark instances.

---

## 🚀 Running Small Benchmark Instances

📌 **Important:** Activate the environment before running the benchmarks:
```bash
conda activate coyoteEnv
```

Then run:
```bash
cd /scratch/<your_user_id>/coyote
python3 run_small_benchmarks.py 
```
