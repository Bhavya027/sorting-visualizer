# 🧠 Smart Sorting Studio

> A full-stack sorting algorithm visualizer powered by a **real C++ backend** — not just a JavaScript simulation.

![C++](https://img.shields.io/badge/C++-17-00599C?style=flat&logo=c%2B%2B&logoColor=white)
![Python](https://img.shields.io/badge/Python-3.8+-3776AB?style=flat&logo=python&logoColor=white)
![Flask](https://img.shields.io/badge/Flask-REST%20API-000000?style=flat&logo=flask&logoColor=white)
![TailwindCSS](https://img.shields.io/badge/Tailwind-CSS-38B2AC?style=flat&logo=tailwind-css&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-green?style=flat)

---

## 📌 About

Smart Sorting Studio lets you visualize seven classic sorting algorithms running on **actual native C++ code**. Your array is sent to a Flask REST API, processed by a compiled C++ binary, and the step-by-step frames are streamed back to the browser — animating as a smooth, real-time bar chart.

Most sorting visualizers animate pre-written JavaScript logic. This one doesn't. The sorting happens in C++, the stats are real, and the execution time is measured using `std::chrono`.

---

## ✨ Features

- 📊 **7 Sorting Algorithms** — Bubble, Selection, Insertion, Merge, Quick, Counting, Radix
- ⚙️ **Real C++ Backend** — native execution via compiled binary, called through Flask
- 🎞️ **Step-by-step Animation** — pause, play, and step through each comparison pass
- 📈 **Live Metrics** — comparisons, swaps, execution time (µs), and stability flag
- 🗂️ **Dataset Presets** — Marks, Sales, Log Codes, or enter your own array
- 🎲 **Random Generator** — generate a shuffled array of 30 values instantly
- 💾 **JSON Export** — download your sort result and stats
- 🟢 **Health Indicator** — nav bar shows if Flask + C++ binary are live
- 🚀 **Deploy-ready** — works locally and on Render / Railway

---

## 🗂️ Project Structure

```
sorting-visualizer/
├── sorting_visualizer.cpp    # C++ sorting engine (all 7 algorithms)
├── sorting_visualizer         # Compiled binary (generated after build)
├── app.py                    # Flask REST API — bridge between frontend & C++
├── index.html                # Frontend UI (Tailwind CSS + Vanilla JS)
└── requirements.txt          # Python dependencies
```

---

## 🚀 Getting Started

### Prerequisites

- `g++` with C++17 support (GCC 9+ or Clang 11+)
- Python 3.8+
- `pip`

---

### Step 1 — Compile the C++ Binary

```bash
g++ -std=c++17 -O2 -o sorting_visualizer sorting_visualizer.cpp
```

> On Windows, the output will be `sorting_visualizer.exe` — `app.py` handles this automatically.

---

### Step 2 — Install Python Dependencies

```bash
pip install flask flask-cors
```

Or using the requirements file:

```bash
pip install -r requirements.txt
```

---

### Step 3 — Start the Flask API

```bash
python app.py
```

Flask starts at `http://localhost:5000`. You'll see a confirmation in the terminal:

```
Smart Sorting Studio — Flask API
Binary  : /path/to/sorting_visualizer
Serving : http://localhost:5000
```

---

### Step 4 — Open the Frontend

Open `index.html` directly in your browser, or use **VS Code Live Server**.

The nav bar will show a green **Backend OK** pill when Flask and the binary are both detected.

---

## 🔌 API Reference

### `POST /sort`

Runs the selected algorithm on the provided array.

**Request Body:**
```json
{
  "data": [78, 91, 45, 62, 88],
  "algo": 1
}
```

**Response:**
```json
{
  "sorted": [45, 62, 78, 88, 91],
  "stats": {
    "algorithm": "Bubble Sort",
    "comparisons": 9,
    "swaps": 5,
    "time_ms": 0.0024,
    "stable": "Yes"
  },
  "frames": [
    [78, 45, 62, 88, 91],
    [45, 78, 62, 88, 91],
    "..."
  ]
}
```

---

### `GET /health`

Returns API status and confirms the C++ binary is found.

```json
{
  "status": "ok",
  "binary_found": true
}
```

---

## 🔢 Algorithm Reference

| # | Algorithm | Time Complexity | Stable |
|---|-----------|----------------|--------|
| 1 | Bubble Sort | O(n²) | ✅ Yes |
| 2 | Selection Sort | O(n²) | ❌ No |
| 3 | Insertion Sort | O(n²) | ✅ Yes |
| 4 | Merge Sort | O(n log n) | ✅ Yes |
| 5 | Quick Sort | O(n log n) avg | ❌ No |
| 6 | Counting Sort | O(n + k) | ✅ Yes |
| 7 | Radix Sort | O(d · (n + k)) | ✅ Yes |

> ⚠️ Counting Sort and Radix Sort require **non-negative integers** only.

---

## ☁️ Deployment (Render.com)

1. Push this repo to GitHub
2. Go to [render.com](https://render.com) → **New Web Service** → connect your repo
3. Set the following in the Render dashboard:

| Setting | Value |
|---------|-------|
| **Build Command** | `g++ -std=c++17 -O2 -o sorting_visualizer sorting_visualizer.cpp && pip install -r requirements.txt` |
| **Start Command** | `python app.py` |

4. Once deployed, copy your Render URL (e.g. `https://your-app.onrender.com`)
5. In `index.html`, update this one line:

```javascript
const API_URL = "https://your-app.onrender.com";
```

---

## 🛠️ How It Works

```
Browser (index.html)
      │
      │  fetch() POST /sort
      ▼
Flask API (app.py)
      │
      │  subprocess.run()
      ▼
C++ Binary (sorting_visualizer)
      │
      │  stdout — sorted array + STATS line + step frames
      ▼
Flask parses output → returns JSON to browser
      │
      ▼
JS animates bar chart frame by frame
```

The C++ binary supports a **headless mode** triggered by CLI flags:

```bash
./sorting_visualizer --data "78 91 45 62" --algo 1
```

This allows Flask to invoke it programmatically and capture the output.

---

## 📦 Tech Stack

| Layer | Technology |
|-------|------------|
| Sorting Engine | C++ 17 (`std::chrono`, `std::vector`) |
| API Server | Python Flask + flask-cors |
| Frontend | HTML5 + Tailwind CSS + Vanilla JS |
| Communication | REST (JSON over HTTP) |
| Deployment | Render / Railway / any VPS |

---

## 📄 License

This project is licensed under the **MIT License** — free to use, modify, and distribute.

---

## 🙌 Acknowledgements

- [Tailwind CSS](https://tailwindcss.com/) for the UI utility classes
- [Google Material Symbols](https://fonts.google.com/icons) for icons
- [Flask](https://flask.palletsprojects.com/) for the lightweight API server
- Inspired by the need to see sorting algorithms work at the **native code level**, not just in the browser

---

<p align="center">Built with C++ · Python Flask · HTML / Tailwind CSS</p>
