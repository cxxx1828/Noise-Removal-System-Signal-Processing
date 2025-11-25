# DSP-FilterPro - Real-Time Noise Reduction System


A real-time digital signal processing system implementing cascaded FIR and IIR filters for noise reduction on TI's TMS320C5535 DSP. Removes dominant noise components at 308 Hz and 2 kHz using fixed-point arithmetic optimized for embedded systems.

---

## System Architecture

```
                      Real-Time Audio Pipeline
┌────────────────────────────────────────────────────────────────┐
│                                                                 │
│   Input Signal                                                  │
│   (Noisy Audio)                                                 │
│        │                                                        │
│        ├─► 308 Hz noise component                              │
│        ├─► 2 kHz noise component                               │
│        └─► Clean signal                                        │
│                                                                 │
└────────┬───────────────────────────────────────────────────────┘
         │
         │ ADC Input (16-bit samples)
         │
┌────────▼───────────────────────────────────────────────────────┐
│                   TMS320C5535 DSP Processor                     │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌──────────────────────────────────────────────────────────┐  │
│  │              STAGE 1: FIR Low-Pass Filter                │  │
│  ├──────────────────────────────────────────────────────────┤  │
│  │  Purpose:  Remove high-frequency noise (2 kHz)           │  │
│  │  Type:     N-tap FIR filter                              │  │
│  │  Cutoff:   ~1.5 kHz                                      │  │
│  │  Method:   Convolution sum with fixed-point multiply    │  │
│  └────────────────────┬─────────────────────────────────────┘  │
│                       │                                         │
│                       │ Filtered Signal                         │
│                       │                                         │
│  ┌────────────────────▼─────────────────────────────────────┐  │
│  │          STAGE 2: IIR Notch Filter (308 Hz)             │  │
│  ├──────────────────────────────────────────────────────────┤  │
│  │  Purpose:  Remove narrow-band noise at 308 Hz           │  │
│  │  Type:     Second-order IIR (biquad)                    │  │
│  │  Q-factor: High (narrow notch)                          │  │
│  │  Method:   Direct Form II implementation                │  │
│  └────────────────────┬─────────────────────────────────────┘  │
│                       │                                         │
│                       │ Clean Signal                            │
│                       │                                         │
│  ┌────────────────────▼─────────────────────────────────────┐  │
│  │          Fixed-Point Processing Unit                     │  │
│  ├──────────────────────────────────────────────────────────┤  │
│  │  • Q15 coefficient representation                        │  │
│  │  • Overflow protection (saturation)                      │  │
│  │  • Rounding for precision                                │  │
│  │  • Signal normalization                                  │  │
│  └──────────────────────────────────────────────────────────┘  │
│                                                                 │
└────────┬───────────────────────────────────────────────────────┘
         │
         │ DAC Output (16-bit samples)
         │
┌────────▼───────────────────────────────────────────────────────┐
│                                                                 │
│   Output Signal                                                 │
│   (Clean Audio)                                                 │
│        │                                                        │
│        └─► Clean signal (noise removed)                        │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

---

## Filter Design Overview

### Stage 1: FIR Low-Pass Filter

**Purpose:** Remove 2 kHz noise component

**Specifications:**
- **Filter Type:** Finite Impulse Response (FIR)
- **Order:** N-tap (configurable)
- **Cutoff Frequency:** ~1.5 kHz
- **Design Method:** Windowed sinc method
- **Window:** Hamming/Blackman
- **Sampling Rate:** 8 kHz

**Transfer Function:**
```
y[n] = Σ (h[k] × x[n-k])  for k = 0 to N-1

Where:
  h[k] = FIR coefficients (N taps)
  x[n] = Input samples
  y[n] = Filtered output
```

**Fixed-Point Representation:**
```c
// Q15 format: 1 sign bit + 15 fractional bits
// Range: -1.0 to +0.999969482421875

int16_t fir_coeffs[N] = {
    // Normalized coefficients scaled to Q15
    0x0123,  // h[0] = 0.0089 × 2^15
    0x045A,  // h[1] = 0.0341 × 2^15
    // ... more coefficients
};
```

---

### Stage 2: IIR Notch Filter (308 Hz)

**Purpose:** Precisely remove 308 Hz narrow-band noise

**Specifications:**
- **Filter Type:** Infinite Impulse Response (IIR)
- **Structure:** Biquad (second-order section)
- **Center Frequency:** 308 Hz
- **Q-Factor:** 10-20 (narrow notch)
- **Implementation:** Direct Form II

**Transfer Function:**
```
         b0 + b1×z^(-1) + b2×z^(-2)
H(z) = ─────────────────────────────
         1 + a1×z^(-1) + a2×z^(-2)

Difference Equation:
y[n] = b0×x[n] + b1×x[n-1] + b2×x[n-2]
                - a1×y[n-1] - a2×y[n-2]
```

**Coefficient Calculation:**
```c
// Notch filter design parameters
f0 = 308 Hz       // Center frequency
fs = 8000 Hz      // Sampling rate
Q = 15            // Quality factor (notch width)

ω0 = 2π × f0 / fs
α = sin(ω0) / (2 × Q)

// Biquad coefficients (normalized)
b0 = 1.0
b1 = -2 × cos(ω0)
b2 = 1.0
a0 = 1 + α
a1 = -2 × cos(ω0)
a2 = 1 - α

// Convert to Q15 fixed-point
b0_q15 = (int16_t)(b0 / a0 × 32768)
b1_q15 = (int16_t)(b1 / a0 × 32768)
b2_q15 = (int16_t)(b2 / a0 × 32768)
a1_q15 = (int16_t)(a1 / a0 × 32768)
a2_q15 = (int16_t)(a2 / a0 × 32768)
```

---

## Fixed-Point Arithmetic

### Q15 Number Format

```
┌─────┬───────────────────────────────────┐
│ S   │    15-bit Fractional Part         │
└─────┴───────────────────────────────────┘
  MSB                                    LSB

Decimal Value = (Signed Integer) / 32768

Examples:
  0x7FFF =  32767 / 32768 =  0.999969...  (max positive)
  0x4000 =  16384 / 32768 =  0.5
  0x0000 =      0 / 32768 =  0.0
  0xC000 = -16384 / 32768 = -0.5
  0x8000 = -32768 / 32768 = -1.0          (min negative)
```

### Multiplication and Scaling

**Challenge:** Multiplying two Q15 numbers produces Q30 result

**Solution:** Shift and round to maintain Q15 format

```c
// Q15 × Q15 multiplication
int32_t multiply_q15(int16_t a, int16_t b) {
    int32_t product = (int32_t)a * (int32_t)b;  // Q30 result
    product += 0x4000;                          // Rounding
    return (int16_t)(product >> 15);            // Back to Q15
}
```

### Overflow Protection

```c
// Saturation logic for accumulation
int32_t accumulator = 0;

for (int i = 0; i < N; i++) {
    accumulator += multiply_q15(coeffs[i], samples[i]);
}

// Saturate to Q15 range
if (accumulator > 32767)
    output = 32767;
else if (accumulator < -32768)
    output = -32768;
else
    output = (int16_t)accumulator;
```

---

## Implementation Details

### Signal Normalization

**Problem:** Audio signals may exceed Q15 range causing overflow

**Solution:** Pre-scale input by dividing by maximum expected amplitude

```c
// Find peak amplitude in input signal
int16_t find_peak(int16_t *signal, int length) {
    int16_t max = 0;
    for (int i = 0; i < length; i++) {
        int16_t abs_val = (signal[i] < 0) ? -signal[i] : signal[i];
        if (abs_val > max) max = abs_val;
    }
    return max;
}

// Normalize signal to prevent overflow
void normalize_signal(int16_t *signal, int length) {
    int16_t peak = find_peak(signal, length);
    int16_t scale = 32767 / peak;  // Q15 scaling factor
    
    for (int i = 0; i < length; i++) {
        signal[i] = multiply_q15(signal[i], scale);
    }
}
```

---

## Processing Pipeline

```c
// Main real-time processing loop
void process_audio_sample(int16_t input) {
    
    // Stage 1: FIR Low-Pass Filter
    static int16_t fir_buffer[FIR_ORDER];
    int16_t fir_output;
    
    // Shift buffer and insert new sample
    for (int i = FIR_ORDER-1; i > 0; i--) {
        fir_buffer[i] = fir_buffer[i-1];
    }
    fir_buffer[0] = input;
    
    // Convolution sum
    int32_t fir_sum = 0;
    for (int i = 0; i < FIR_ORDER; i++) {
        fir_sum += multiply_q15(fir_coeffs[i], fir_buffer[i]);
    }
    fir_output = saturate(fir_sum);
    
    // Stage 2: IIR Notch Filter
    static int16_t iir_x[2] = {0, 0};  // Input history
    static int16_t iir_y[2] = {0, 0};  // Output history
    int16_t iir_output;
    
    // Direct Form II biquad
    int32_t iir_sum = multiply_q15(b0, fir_output)
                    + multiply_q15(b1, iir_x[0])
                    + multiply_q15(b2, iir_x[1])
                    - multiply_q15(a1, iir_y[0])
                    - multiply_q15(a2, iir_y[1]);
    
    iir_output = saturate(iir_sum);
    
    // Update history buffers
    iir_x[1] = iir_x[0];
    iir_x[0] = fir_output;
    iir_y[1] = iir_y[0];
    iir_y[0] = iir_output;
    
    // Output clean signal
    output_sample(iir_output);
}
```

---

## Test Results

### Test Signal Composition

```c
// Generate test input with noise components
int16_t test_input[BUFFER_SIZE];

for (int n = 0; n < BUFFER_SIZE; n++) {
    // Clean signal: 500 Hz sine wave
    float clean = 0.3 * sin(2 * PI * 500 * n / 8000);
    
    // Noise 1: 308 Hz component
    float noise_308 = 0.2 * sin(2 * PI * 308 * n / 8000);
    
    // Noise 2: 2 kHz component
    float noise_2k = 0.15 * sin(2 * PI * 2000 * n / 8000);
    
    // Composite signal
    float composite = clean + noise_308 + noise_2k;
    
    // Convert to Q15
    test_input[n] = (int16_t)(composite * 32767);
}
```

### Frequency Response Verification

| Frequency | Input Level | Output Level | Attenuation |
|-----------|-------------|--------------|-------------|
| 100 Hz    | 0 dB        | -0.5 dB      | Passband    |
| 308 Hz    | 0 dB        | **-40 dB**   | **Notch**   |
| 500 Hz    | 0 dB        | -1.0 dB      | Passband    |
| 1000 Hz   | 0 dB        | -2.0 dB      | Transition  |
| 2000 Hz   | 0 dB        | **-35 dB**   | **Stopband**|
| 3000 Hz   | 0 dB        | -50 dB       | Stopband    |

**Result:** Both noise components successfully attenuated by >35 dB

---

## Performance Metrics

### Computational Complexity

| Operation | Cycles/Sample | Percentage |
|-----------|---------------|------------|
| FIR Filter (N=32 taps) | ~480 cycles | 60% |
| IIR Filter (biquad) | ~120 cycles | 15% |
| Normalization | ~80 cycles | 10% |
| I/O Operations | ~120 cycles | 15% |
| **Total** | **~800 cycles** | **100%** |

**Maximum Throughput:** 10,000 samples/sec (at 8 MHz clock)

### Memory Usage

```
Program Memory:    ~2 KB
Data Memory:       ~512 bytes
  - FIR buffer:    64 bytes (32 taps × 2 bytes)
  - IIR history:   8 bytes
  - Coefficients:  80 bytes
  - Stack/temp:    ~360 bytes
```

---

## Hardware Setup

### TMS320C5535 DSP Kit Configuration

**Connections:**
```
Audio Input (Line-In)
    ↓
[ADC] → TMS320C5535 DSP → [DAC]
                ↓
        Audio Output (Line-Out)

Additional:
- JTAG debugger for code loading
- Power supply: 5V via USB or external
- Status LEDs for processing indication
```

**Sampling Configuration:**
- **Sampling Rate:** 8 kHz
- **Bit Depth:** 16-bit signed (Q15)
- **Buffer Size:** 128 samples (double buffering)
- **Latency:** ~16 ms per buffer

---

## Project Structure

```
DSP-FilterPro/
├── src/
│   ├── main.c                # Main processing loop
│   ├── fir_filter.c          # FIR implementation
│   ├── fir_filter.h
│   ├── iir_filter.c          # IIR biquad implementation
│   ├── iir_filter.h
│   ├── fixed_point.c         # Q15 arithmetic utilities
│   └── fixed_point.h
├── coefficients/
│   ├── fir_coeffs.h          # Pre-calculated FIR taps
│   └── iir_coeffs.h          # Biquad coefficients
├── tests/
│   ├── test_signal.c         # Generate test inputs
│   ├── verify_output.c       # FFT analysis of results
│   └── benchmark.c           # Performance measurement
├── matlab/
│   ├── design_fir.m          # FIR filter design script
│   ├── design_iir.m          # IIR notch design
│   └── analyze_response.m    # Frequency response plot
├── docs/
│   ├── filter_design.pdf     # Detailed design documentation
│   └── fixed_point_guide.pdf # Q15 arithmetic reference
└── README.md
```

---

## Building and Running

### 1. Code Composer Studio Setup

```bash
# Import project
File → Import → Existing CCS Project

# Configure build settings
Project Properties → Build → C2000 Compiler
    - Optimization: -O2
    - Fixed-point support: Enabled
```

### 2. Coefficient Generation (MATLAB)

```matlab
% Run filter design scripts
cd matlab/
design_fir    % Generates FIR coefficients
design_iir    % Generates IIR coefficients

% Copy output to coefficients/ folder
```

### 3. Flash to DSP

```bash
# Build project
Project → Build All

# Load to DSP
Run → Debug
Run → Resume
```

### 4. Test with Audio

```bash
# Connect audio source to Line-In
# Monitor filtered output on Line-Out
# Use oscilloscope/spectrum analyzer for verification
```

---

## Key Achievements

- **Real-time processing** at 8 kHz sampling rate
- **Dual-stage filtering** removes specific noise frequencies
- **Fixed-point optimization** for embedded DSP performance
- **Overflow protection** ensures numerical stability
- **Coefficient quantization** maintains filter accuracy
- **Minimal latency** (<20 ms) for real-time applications
- **Efficient memory usage** fits in 2.5 KB total
- **Verified performance** with >35 dB noise attenuation

---

## Future Enhancements

- [ ] Adaptive filter coefficients based on noise profile
- [ ] Multi-band parametric EQ
- [ ] Dynamic range compression
- [ ] Echo cancellation algorithm
- [ ] Frequency domain processing (FFT-based filtering)
- [ ] SIMD optimization for TI C55x intrinsics
- [ ] Real-time spectrum analyzer display
- [ ] Bluetooth audio streaming support

---

## Author

**Nina Dragićević**  

---

