# 🚀 SMC3 Firmware Modification: Feedback Magnification (1.8x)

This version of the **SMC3.ino** firmware has been modified to increase the sensitivity of the potentiometer feedback. This is specifically optimized for 2-DOF "Platform Mover" rigs where the physical rotation of the motor arms (and thus the potentiometers) is restricted.

## 📌 The "Why"
In a standard build, the Arduino reads the full 0-1023 bit range of a potentiometer. If your mechanical setup only uses ~55% of the POT's rotation, you are wasting resolution. By "magnifying" the feedback, we stretch that 55% physical travel to cover 100% of the software range, resulting in:
* **Higher Precision:** More granular control for fine road textures and small telemetry cues.
* **Sharper Response:** The PID loop reacts faster to smaller physical changes.

## 🛠 Technical Changes
The standard `analogRead()` calls in the main motor update loop have been replaced with a `map()` and `constrain()` block.

### Before:
```cpp
Feedback1 = analogRead(FeedbackPin1);

After (1.8x Magnification):
We target the center 569 units of the POT (range 227 to 796) and scale it to the full 1024 units (0-1023).

C++
// 1.8X Magnification Logic
Feedback1 = map(analogRead(FeedbackPin1), 227, 796, 0, 1023);
Feedback1 = constrain(Feedback1, 0, 1023);
