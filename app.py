"""
app.py — Flask bridge between Smart Sorting Studio frontend and C++ binary
Run:  python app.py
"""

from flask import Flask, request, jsonify
from flask_cors import CORS
import subprocess, os, json

app = Flask(__name__)
CORS(app)  # allow requests from the HTML file opened in browser

# ── Path to compiled C++ binary ──────────────────────────────────────────────
# Looks for it next to this file; change the name if yours differs
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
CPP_BINARY = os.path.join(BASE_DIR, "sorting_visualizer")
# On Windows the binary is sorting_visualizer.exe
if os.name == "nt":
    CPP_BINARY += ".exe"


# ── /sort  POST ───────────────────────────────────────────────────────────────
@app.route("/sort", methods=["POST"])
def sort():
    body = request.get_json(force=True)
    data: list = body.get("data", [])
    algo: int  = int(body.get("algo", 1))

    if not data:
        return jsonify({"error": "No data provided"}), 400
    if algo < 1 or algo > 7:
        return jsonify({"error": "algo must be 1-7"}), 400

    data_str = " ".join(map(str, data))

    try:
        proc = subprocess.run(
            [CPP_BINARY, "--data", data_str, "--algo", str(algo)],
            capture_output=True, text=True, timeout=15
        )
    except FileNotFoundError:
        return jsonify({"error": f"Binary not found: {CPP_BINARY}. Did you compile the C++?"}), 500
    except subprocess.TimeoutExpired:
        return jsonify({"error": "C++ process timed out"}), 500

    output = proc.stdout

    # ── Parse sorted array ────────────────────────────────────────────────────
    sorted_arr = []
    for line in output.splitlines():
        if line.startswith("Sorted"):
            try:
                sorted_arr = list(map(int, line.split(":")[1].split()))
            except Exception:
                pass
            break

    # ── Parse stats line:  STATS|Name|comparisons|swaps|ms|Stable ────────────
    stats = {}
    for line in output.splitlines():
        if line.startswith("STATS|"):
            p = line.split("|")
            stats = {
                "algorithm":   p[1],
                "comparisons": int(p[2]),
                "swaps":       int(p[3]),
                "time_ms":     float(p[4]),
                "stable":      p[5].strip()   # "Yes" or "No"
            }
            break

    # ── Build step frames for bar animation ───────────────────────────────────
    # Each "Step N" / "Pass N" / "Merge[..]" / "Partition[..]" line is a frame
    frames = []
    for line in output.splitlines():
        for prefix in ("  Pass ", "  Step ", "  Merge[", "  Partition[",
                       "  Digit pass ", "  Building"):
            if line.startswith(prefix):
                try:
                    nums = list(map(int, line.split(":")[1].split()))
                    if nums:
                        frames.append(nums)
                except Exception:
                    pass
                break

    return jsonify({
        "sorted": sorted_arr,
        "stats":  stats,
        "frames": frames          # list of arrays, one per visualisation step
    })


# ── /health GET ───────────────────────────────────────────────────────────────
@app.route("/health", methods=["GET"])
def health():
    binary_ok = os.path.isfile(CPP_BINARY)
    return jsonify({"status": "ok", "binary_found": binary_ok})


if __name__ == "__main__":
    port = int(os.environ.get("PORT", 5000))
    print(f"\n  Smart Sorting Studio — Flask API")
    print(f"  Binary  : {CPP_BINARY}")
    print(f"  Serving : http://localhost:{port}\n")
    app.run(host="0.0.0.0", port=port, debug=True)
