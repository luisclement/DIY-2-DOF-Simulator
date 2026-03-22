## ⚙️ SMC3 Tuning Parameters & Setup Guide

This guide covers the configuration of the SMC3 Arduino Motion Controller using the Windows SMC3 Utility.

> [!IMPORTANT]
> **Safety First:** Perform initial tuning with the Windows SMC3 Utility **before** connecting to Simtools or any other simulation software.

---

### 🛡️ Safety & Limits

| Parameter | Description |
| :--- | :--- |
| **Clip Input** | Creates a buffer zone (0-255). If feedback enters this zone, the SMC3 applies hard reverse braking. Must be $\ge$ Max Limit. |
| **Max Limits** | The "Kill Switch" zone (0-255). If motors move past this point, the driver shuts down until a manual reset occurs. |
| **Deadzone** | Creates a hysteresis zone. If you need a value > 1, check your mechanical hardware for "slop" or loose bolts. |

---

### 🎮 PID Control Parameters

* **Fpid (Update Divider):** The PID loop runs at 4000Hz by default. Use this divider to reduce frequency if motor windings produce audible high-pitched noise.
* **Kp (Proportional):** The main strength parameter (Range 0-1000). Higher values drive the motor harder to reach the target. Typical values: **300–500**.
* **Ki (Integral):** Helps reach the exact target position. Usually set to **0** if `Kp` and `PWMmin` are tuned correctly. High values cause oscillation.
* **Kd (Derivative):** Acts as a "damper" to reduce overshoot during fast movements. High values can make the motor feel sluggish.
* **Ks (Smoothing):** A divider for the `Kd` term. A value of 5 means `Kd` updates once every 5 PID cycles.

---

### ⚡ PWM & Power Settings

* **Fpwm:** Sets the Motor PWM frequency. Note that Motors 1 & 2 share a frequency due to Arduino Uno hardware limitations.
* **PWMmin:** The minimum power needed to overcome friction and get the motor moving (0-255). 
* **PWMmax:** The maximum power cap allowed for the motors.
* **PWMrev:** The power used for "Hard Braking" when entering the Clip zone. Set to **0** to disable reverse braking and use standard PID logic instead.

---

### 📡 Connecting to Simtools via UDP

Simtools can send motion data via **UDP** over your network. This allows the SMC3 Utility to act as a "middle-man," enabling:
1.  **Real-time monitoring** of motor graphs while gaming.
2.  **Live tuning** without needing a second physical serial port.

---

## 🛠 Setup Checklist

Follow these steps in order to prevent hardware damage:

### 1. Pre-Power Check
- [ ] Disconnect mechanical linkages (motors should spin freely).
- [ ] Manually turn feedback potentiometers; verify the green line in the Utility moves correctly.
- [ ] Set `PWMmax` to a low safety value (e.g., 60).

### 2. Direction & Braking
- [ ] Power the motors. If they "run away" from the target, reverse the motor wires.
- [ ] Move the motor into the **Clip Zone** and ensure `PWMrev` pushes it back toward the center.

### 3. PID Tuning Sequence
- [ ] Set `Ki` and `Kd` to **0**.
- [ ] Increase `Kp` until the motor follows the target line closely but doesn't shake.
- [ ] Increase `Kd` slightly to stop any "bounce" or overshoot at the end of a movement.
- [ ] Run "Sine Wave" tests in the Utility to check for heat and stability.

### 4. Integration
- [ ] Reattach mechanical linkages.
- [ ] Configure Simtools UDP output to the IP address of the PC running SMC3 Utility.
