import serial
import csv
import time
import datetime
import matplotlib.pyplot as plt

# ── Configuration ──────────────────────────────────────────────────
PORT     = 'COM6'      # Change to your port number
BAUD     = 9600        # Must match Serial.begin(9600) in Arduino
FILENAME = 'environment_log.csv'
DURATION = 1800        # Seconds to log (1800 = 30 minutes)

# ── Open serial connection ──────────────────────────────────────────
print(f"Connecting to {PORT}...")
ser = serial.Serial(PORT, BAUD, timeout=2)
time.sleep(2)          # Wait for Arduino to reset after connection
print(f"Connected. Logging for {DURATION} seconds.")
print("Press Ctrl+C to stop early.\n")

rows  = []
start = time.time()

# ── Data collection loop ────────────────────────────────────────────
try:
    while time.time() - start < DURATION:
        line = ser.readline().decode('utf-8').strip()

        if ',' in line:
            parts = line.split(',')
            if len(parts) == 3:
                try:
                    temp  = float(parts[0])
                    humid = float(parts[1])
                    raw   = int(parts[2])
                    volts = round(raw * (5.0 / 1023.0), 3)
                    ts    = datetime.datetime.now().strftime('%H:%M:%S')

                    rows.append([ts, temp, humid, raw, volts])
                    print(f"{ts}  Temp:{temp:.1f}C  "
                          f"Humid:{humid:.0f}%  "
                          f"Light:{raw} ({volts:.2f}V)")
                except ValueError:
                    pass

except KeyboardInterrupt:
    print("\nStopped early by user.")

ser.close()
print(f"\n{len(rows)} readings collected.")

# ── Save to CSV ─────────────────────────────────────────────────────
with open(FILENAME, 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(['time', 'temp_c', 'humid_pct',
                     'light_raw', 'light_volts'])
    writer.writerows(rows)

print(f"Saved: {FILENAME}")

# ── Plot graph ──────────────────────────────────────────────────────
if not rows:
    print("No data to plot.")
else:
    times  = [r[0] for r in rows]
    temps  = [r[1] for r in rows]
    humids = [r[2] for r in rows]
    volts  = [r[4] for r in rows]

    step = max(1, len(times) // 10)

    fig, (ax1, ax2, ax3) = plt.subplots(3, 1, figsize=(12, 8))
    fig.suptitle('Environment Monitor — 30 Minute Log', fontsize=14)

    ax1.plot(times, temps, color='tomato', linewidth=1.5)
    ax1.set_ylabel('Temperature (°C)')
    ax1.set_title('Temperature')
    ax1.grid(True, alpha=0.3)

    ax2.plot(times, humids, color='steelblue', linewidth=1.5)
    ax2.set_ylabel('Humidity (%RH)')
    ax2.set_title('Humidity')
    ax2.grid(True, alpha=0.3)

    ax3.plot(times, volts, color='goldenrod', linewidth=1.5)
    ax3.set_ylabel('Light (V)')
    ax3.set_title('Ambient Light Level')
    ax3.set_xlabel('Time')
    ax3.grid(True, alpha=0.3)

    for ax in [ax1, ax2, ax3]:
        ax.set_xticks(ax.get_xticks()[::step])
        plt.setp(ax.get_xticklabels(), rotation=45, ha='right')

    plt.tight_layout()
    plt.savefig('environment_graph.png', dpi=150, bbox_inches='tight')
    print("Graph saved: environment_graph.png")
    plt.show()