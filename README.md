# noisy-logue
A simple, enveloped noise oscillator for the Korgue *logue series of synths.

## What?
The noise mode of the Multi oscillator on the Korg Prologue provides a wide array of shapeable noise options. One major oversight in my eyes, however, is that the noise level
in the mixer section is fixed -- we can't assign an envelope or LFO to modulate the noise level over time.

noisy-logue aims to create a noise engine that can do almost everything the default noise engine can, while also unlocking additional modulation capabilities and remaining easy
to use. It includes a built-in exponential ADSR envelope that can be used to modulate the noise level or other noise generation parameters. It also allows for selecting a 
different basenoise color -- choose from white, pink, red, and 'decimated' noise types, and then tweak the filter and sample rate modulation.

## How?
noisy-logue was made to be easy to use. Want a punch of white noise right at the beginning of your sound? Select Noisy from the USR mode of the Multi oscillator, then adjust Shape
to dial in your desired decay envelope. That's it!

Want that noise to stick around? You can easily add a sustain phase to your envelope by holding SHIFT and turning the Shape knob to dial in your sustain level. By default, 
release time will be equal to decay time, keeping your sound plucky when notes are played quickly.

If you're looking for even more flexibility, you can dive into the Multi Engine settings page (Edit Mode > button 7). Here are the available parameters:

| Name            | Use |
| --------------- | --- |
| Attack          | The length of the attack phase of the envelope. 100% ≈ 5 seconds. |
| Release         | A scaling factor for the release time, compared to the decay time. <br> 0% = decay time. <br> -100% = immediate release. <br> +100% = decay time + ~5 seconds. |
| Noise type      | 1 = white <br> 2 = pink <br> 3 = red <br> 4 = decim (sample and hold) |
| Noise mod       | For "decim" noise type, this is the sample and hold rate. Higher values provide lower sample rates; negative values are ignored. <br> For other noise types, this is a DJ-style filter. Negative values are low pass; positive values are high pass. |
| LFO target      | Determines which parameter(s) the front panel LFO affects, if targeting the Shape parameter. <br> 1 = none <br> 2 = volume <br> 3 = "noise mod" parameter above <br> 4 = volume **and** noise mod
| Env -> Mod      | If non-zero, the envelope is disconnected from the oscillator level and instead controls the "Noise mod" parameter. The value determines the modulation depth and polarity. |

**IMPORTANT NOTE:** Due to a workaround for a bug in Prologue firmware (and possibly other *logue hardware), any values set to -100% will behave as if they were set to 0%.
Initialized patches may also have some values set to -100% instead of 0%. I apologize for the inconvenience; I've followed up with Korg to hopefully get this fixed in future
firmware revisions.

