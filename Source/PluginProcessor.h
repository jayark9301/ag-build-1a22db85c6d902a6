#pragma once
#include <JuceHeader.h>
class VelvetAudioProcessor : public juce::AudioProcessor {
public:
    VelvetAudioProcessor();
    ~VelvetAudioProcessor() override;
    void prepareToPlay(double, int) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout&) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return "Velvet"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 2.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;
    juce::AudioProcessorValueTreeState apvts;
private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    double currentSampleRate = 44100.0;
    juce::dsp::Gain<float> gain_DJ2F;
    juce::dsp::Compressor<float> comp_C87U;
    juce::dsp::Gain<float> compMakeup_C87U;
    juce::dsp::Reverb reverb_B4W8;
    juce::dsp::DryWetMixer<float> reverbMix_B4W8;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>,juce::dsp::IIR::Coefficients<float>> bbShelf_0W9Q;
    // bitcrusher_AE6S inline
    // stereowidth_8H5C inline
    juce::dsp::Gain<float> inputGainDsp, outputGainDsp;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VelvetAudioProcessor)
};
