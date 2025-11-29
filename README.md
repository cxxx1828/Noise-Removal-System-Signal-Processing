# DSP-FilterPro - Real-Time Noise Reduction

Real-time audio filtering on TI's TMS320C5535 DSP. Removes noise at 308 Hz and 2 kHz using cascaded FIR and IIR filters with fixed-point arithmetic.

## How It Works

Two-stage filtering pipeline:

**Stage 1: FIR Low-Pass Filter**
- Removes 2 kHz high-frequency noise
- Cutoff around 1.5 kHz
- Windowed sinc design (Hamming/Blackman window)

**Stage 2: IIR Notch Filter**
- Removes 308 Hz narrow-band noise
- Second-order biquad structure
- High Q-factor for narrow notch

Both filters use Q15 fixed-point format (1 sign bit + 15 fractional bits) optimized for the DSP's 16-bit architecture.

## Filter Design

**FIR Filter:**
Uses convolution sum over N taps. Each output sample is the weighted sum of the current input and N-1 previous inputs.

**IIR Biquad:**
Second-order recursive filter. Output depends on current/previous inputs and previous outputs. Coefficients calculated for 308 Hz notch at 8 kHz sampling rate with Q=15.

## Fixed-Point Arithmetic

Q15 format represents values from -1.0 to ~0.999 by dividing a signed 16-bit integer by 32768.

Multiplying two Q15 numbers produces a 32-bit result that needs to be shifted and rounded back to Q15. Overflow protection uses saturation to prevent clipping.

## Test Results

Frequency response with composite test signal (500 Hz clean + 308 Hz noise + 2 kHz noise):

| Frequency | Attenuation |
|-----------|-------------|
| 100 Hz    | -0.5 dB (passband) |
| 308 Hz    | **-40 dB** (notch) |
| 500 Hz    | -1.0 dB (passband) |
| 2000 Hz   | **-35 dB** (stopband) |

Both noise components successfully removed (>35 dB attenuation).

## Performance

Computational load per sample:
- FIR filter (32 taps): ~480 cycles (60%)
- IIR biquad: ~120 cycles (15%)
- Normalization: ~80 cycles (10%)
- I/O: ~120 cycles (15%)
- **Total: ~800 cycles**

Maximum throughput: 10,000 samples/sec at 8 MHz clock

Memory usage:
- Program: ~2 KB
- Data: ~512 bytes (buffers, coefficients, stack)

## Hardware Setup

TMS320C5535 DSP configuration:
- Sampling rate: 8 kHz
- Bit depth: 16-bit signed
- Buffer: 128 samples (double buffered)
- Latency: ~16 ms

Audio goes in through ADC, gets processed, comes out through DAC.

## Project Structure

```
DSP-FilterPro/
├── src/
│   ├── main.c
│   ├── fir_filter.c/h
│   ├── iir_filter.c/h
│   └── fixed_point.c/h
├── coefficients/
│   ├── fir_coeffs.h
│   └── iir_coeffs.h
├── tests/
│   ├── test_signal.c
│   ├── verify_output.c
│   └── benchmark.c
├── matlab/
│   ├── design_fir.m
│   ├── design_iir.m
│   └── analyze_response.m
└── docs/
    ├── filter_design.pdf
    └── fixed_point_guide.pdf
```

## Build and Run

Generate coefficients in MATLAB by running the design scripts in the matlab/ directory. These calculate the FIR taps and IIR biquad coefficients.

Build in Code Composer Studio - import the project, set optimization to -O2, enable fixed-point support, and flash to the DSP.

Connect audio source to line-in and monitor filtered output on line-out. Use a spectrum analyzer to verify noise removal.

## What Could Be Added

- Adaptive coefficients based on noise profile
- Multi-band parametric EQ
- Dynamic range compression
- Echo cancellation
- FFT-based filtering
- Real-time spectrum display

## Author

Nina Dragićević

## License

MIT
