import serial
import matplotlib.pyplot as plt
import math

PORT = 'COM6'
BAUD = 9600

ser = serial.Serial(PORT, BAUD, timeout=1)

fig = plt.figure(figsize=(8, 8))
ax = fig.add_subplot(111, projection='polar')
ax.set_title('Ultrasonic Radar Sweep', pad=20)
ax.set_ylim(0, 200)
ax.set_theta_zero_location('N')

angles = []
distances = []

plt.ion()

try:
    while True:
        line = ser.readline().decode('utf-8').strip()
        if ',' in line:
            parts = line.split(',')
            if len(parts) == 2:
                try:
                    angle_deg  = float(parts[0])
                    distance   = float(parts[1])
                    angle_rad  = math.radians(angle_deg)

                    angles.append(angle_rad)
                    distances.append(min(distance, 200))

                    if len(angles) > 150:
                        angles.pop(0)
                        distances.pop(0)

                    ax.clear()
                    ax.set_ylim(0, 200)
                    ax.set_title('Ultrasonic Radar Sweep', pad=20)
                    ax.set_theta_zero_location('N')
                    ax.plot(angles, distances, 'go-',
                            markersize=3, linewidth=1)
                    ax.fill(angles, distances,
                            alpha=0.15, color='green')
                    plt.pause(0.01)

                except ValueError:
                    pass

except KeyboardInterrupt:
    print("Stopped.")
    ser.close()
    plt.ioff()
    plt.savefig('radar_sweep.png', dpi=150, bbox_inches='tight')
    plt.show()