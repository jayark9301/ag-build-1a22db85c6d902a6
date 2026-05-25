#pragma once
#include "PluginProcessor.h"
#include "CustomLookAndFeel.h"
class VelvetAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
    VelvetAudioProcessorEditor(VelvetAudioProcessor&);
    ~VelvetAudioProcessorEditor() override;
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    VelvetAudioProcessor& processorRef;
    VelvetLookAndFeel lnf;
    juce::Slider knob_INPUT_GAIN;
    juce::Label lbl_INPUT_GAIN;
    juce::AudioProcessorValueTreeState::SliderAttachment att_INPUT_GAIN;
    juce::Slider knob_GAIN_GAIN_DB_DJ2F;
    juce::Label lbl_GAIN_GAIN_DB_DJ2F;
    juce::AudioProcessorValueTreeState::SliderAttachment att_GAIN_GAIN_DB_DJ2F;
    juce::Slider knob_COMPRESSOR_THRESHOLD_C87U;
    juce::Label lbl_COMPRESSOR_THRESHOLD_C87U;
    juce::AudioProcessorValueTreeState::SliderAttachment att_COMPRESSOR_THRESHOLD_C87U;
    juce::Slider knob_COMPRESSOR_RATIO_C87U;
    juce::Label lbl_COMPRESSOR_RATIO_C87U;
    juce::AudioProcessorValueTreeState::SliderAttachment att_COMPRESSOR_RATIO_C87U;
    juce::Slider knob_COMPRESSOR_ATTACK_C87U;
    juce::Label lbl_COMPRESSOR_ATTACK_C87U;
    juce::AudioProcessorValueTreeState::SliderAttachment att_COMPRESSOR_ATTACK_C87U;
    juce::Slider knob_COMPRESSOR_RELEASE_C87U;
    juce::Label lbl_COMPRESSOR_RELEASE_C87U;
    juce::AudioProcessorValueTreeState::SliderAttachment att_COMPRESSOR_RELEASE_C87U;
    juce::Slider knob_COMPRESSOR_MAKEUP_C87U;
    juce::Label lbl_COMPRESSOR_MAKEUP_C87U;
    juce::AudioProcessorValueTreeState::SliderAttachment att_COMPRESSOR_MAKEUP_C87U;
    juce::Slider knob_REVERB_SIZE_B4W8;
    juce::Label lbl_REVERB_SIZE_B4W8;
    juce::AudioProcessorValueTreeState::SliderAttachment att_REVERB_SIZE_B4W8;
    juce::Slider knob_REVERB_DAMP_B4W8;
    juce::Label lbl_REVERB_DAMP_B4W8;
    juce::AudioProcessorValueTreeState::SliderAttachment att_REVERB_DAMP_B4W8;
    juce::Slider knob_REVERB_MIX_B4W8;
    juce::Label lbl_REVERB_MIX_B4W8;
    juce::AudioProcessorValueTreeState::SliderAttachment att_REVERB_MIX_B4W8;
    juce::Slider knob_BASSBOOSTER_FREQ_0W9Q;
    juce::Label lbl_BASSBOOSTER_FREQ_0W9Q;
    juce::AudioProcessorValueTreeState::SliderAttachment att_BASSBOOSTER_FREQ_0W9Q;
    juce::Slider knob_BASSBOOSTER_GAIN_0W9Q;
    juce::Label lbl_BASSBOOSTER_GAIN_0W9Q;
    juce::AudioProcessorValueTreeState::SliderAttachment att_BASSBOOSTER_GAIN_0W9Q;
    juce::Slider knob_BASSBOOSTER_DRIVE_0W9Q;
    juce::Label lbl_BASSBOOSTER_DRIVE_0W9Q;
    juce::AudioProcessorValueTreeState::SliderAttachment att_BASSBOOSTER_DRIVE_0W9Q;
    juce::Slider knob_BITCRUSHER_BITS_AE6S;
    juce::Label lbl_BITCRUSHER_BITS_AE6S;
    juce::AudioProcessorValueTreeState::SliderAttachment att_BITCRUSHER_BITS_AE6S;
    juce::Slider knob_BITCRUSHER_RATEDIV_AE6S;
    juce::Label lbl_BITCRUSHER_RATEDIV_AE6S;
    juce::AudioProcessorValueTreeState::SliderAttachment att_BITCRUSHER_RATEDIV_AE6S;
    juce::Slider knob_BITCRUSHER_MIX_AE6S;
    juce::Label lbl_BITCRUSHER_MIX_AE6S;
    juce::AudioProcessorValueTreeState::SliderAttachment att_BITCRUSHER_MIX_AE6S;
    juce::Slider knob_STEREOWIDTH_WIDTH_8H5C;
    juce::Label lbl_STEREOWIDTH_WIDTH_8H5C;
    juce::AudioProcessorValueTreeState::SliderAttachment att_STEREOWIDTH_WIDTH_8H5C;
    juce::Slider knob_OUTPUT_GAIN;
    juce::Label lbl_OUTPUT_GAIN;
    juce::AudioProcessorValueTreeState::SliderAttachment att_OUTPUT_GAIN;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VelvetAudioProcessorEditor)
};
