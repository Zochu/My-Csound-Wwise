
<Cabbage>

form caption("SimpleSourcePlugin") size(128, 192), colour(127, 127, 127), pluginid("3000") 

hslider bounds(0, 64, 128, 128)channel("Pan")range(0, 1, 0.5, 1, 0.01)
rslider bounds(0, 0, 64, 64) channel("Gain")range(0, 1, 0.5, 1, 0.01)

rslider bounds(64, 0, 64, 64) channel("Freq")range(20, 20000, 440, 0.5, 20)


</Cabbage> 
<CsoundSynthesizer>
<CsOptions>
-n -d 
</CsOptions>
<CsInstruments>
; Initialize the global variables. 
sr = 48000
ksmps = 32
nchnls = 2
0dbfs = 1

instr 1
	kPan chnget "Pan"
	kGain chnget "Gain"
	kFreq chnget "Freq"
	
	a1 oscili 0.5, kFreq

    outs (a1 * abs(kPan-1)) * kGain, (a1 * kPan) * kGain
endin

</CsInstruments>
<CsScore>
i1 0 z
f 0 z
</CsScore>
</CsoundSynthesizer>