#include "PluginProcessor.h"
#include "PluginEditor.h"
VelvetAudioProcessor::VelvetAudioProcessor()
    : AudioProcessor(BusesProperties().withInput("Input",juce::AudioChannelSet::stereo()).withOutput("Output",juce::AudioChannelSet::stereo())),
      apvts(*this,nullptr,"Parameters",createParameterLayout()) {}
VelvetAudioProcessor::~VelvetAudioProcessor() {}
juce::AudioProcessorValueTreeState::ParameterLayout VelvetAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>("INPUT_GAIN","Input Gain",juce::NormalisableRange<float>(-24.0f,24.0f,0.01f),0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("OUTPUT_GAIN","Output Gain",juce::NormalisableRange<float>(-24.0f,24.0f,0.01f),0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("GAIN_GAIN_DB_DJ2F","Gain Gain",juce::NormalisableRange<float>(-24.0f,24.0f,0.01f),0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("COMPRESSOR_THRESHOLD_C87U","Compressor Threshold",juce::NormalisableRange<float>(-60.0f,0.0f,0.01f),-18.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("COMPRESSOR_RATIO_C87U","Compressor Ratio",juce::NormalisableRange<float>(1.0f,20.0f,0.1f),4.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("COMPRESSOR_ATTACK_C87U","Compressor Attack",juce::NormalisableRange<float>(0.1f,200.0f,0.01f),10.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("COMPRESSOR_RELEASE_C87U","Compressor Release",juce::NormalisableRange<float>(10.0f,2000.0f,0.01f),150.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("COMPRESSOR_MAKEUP_C87U","Compressor Makeup",juce::NormalisableRange<float>(0.0f,24.0f,0.01f),0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("REVERB_SIZE_B4W8","Reverb Size",juce::NormalisableRange<float>(0.0f,100.0f,0.01f),60.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("REVERB_DAMP_B4W8","Reverb Damping",juce::NormalisableRange<float>(0.0f,100.0f,0.01f),40.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("REVERB_MIX_B4W8","Reverb Mix",juce::NormalisableRange<float>(0.0f,100.0f,0.01f),40.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BASSBOOSTER_FREQ_0W9Q","Bass Boost Freq",juce::NormalisableRange<float>(40.0f,400.0f,1.0f,0.5f),120.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BASSBOOSTER_GAIN_0W9Q","Bass Boost Gain",juce::NormalisableRange<float>(0.0f,18.0f,0.01f),6.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BASSBOOSTER_DRIVE_0W9Q","Bass Boost Drive",juce::NormalisableRange<float>(0.0f,100.0f,0.01f),20.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BITCRUSHER_BITS_AE6S","Bit Crush Bits",juce::NormalisableRange<float>(2.0f,16.0f,0.01f),8.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BITCRUSHER_RATEDIV_AE6S","Bit Crush Rate Div",juce::NormalisableRange<float>(1.0f,32.0f,0.01f),4.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("BITCRUSHER_MIX_AE6S","Bit Crush Mix",juce::NormalisableRange<float>(0.0f,100.0f,0.01f),80.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("STEREOWIDTH_WIDTH_8H5C","Width Width",juce::NormalisableRange<float>(0.0f,200.0f,0.01f),120.0f));
    return layout;
}
void VelvetAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    currentSampleRate = sampleRate;
    juce::dsp::ProcessSpec spec;
    spec.sampleRate=(uint32_t)sampleRate; spec.maximumBlockSize=(uint32_t)samplesPerBlock; spec.numChannels=(uint32_t)getTotalNumOutputChannels();
    inputGainDsp.prepare(spec); inputGainDsp.setGainDecibels(0.0f);
    outputGainDsp.prepare(spec); outputGainDsp.setGainDecibels(0.0f);
    gain_DJ2F.prepare(spec); gain_DJ2F.setGainDecibels(0.0f);
    comp_C87U.prepare(spec); comp_C87U.setThreshold(-18.0f); comp_C87U.setRatio(4.0f); comp_C87U.setAttack(10.0f); comp_C87U.setRelease(150.0f); compMakeup_C87U.prepare(spec); compMakeup_C87U.setGainDecibels(0.0f);
    { juce::dsp::Reverb::Parameters rp; rp.roomSize=0.6f; rp.damping=0.4f; rp.wetLevel=0.4f; rp.dryLevel=0.6f; reverb_B4W8.setParameters(rp); reverb_B4W8.prepare(spec); }
    *bbShelf_0W9Q.state=*juce::dsp::IIR::Coefficients<float>::makeLowShelf(spec.sampleRate,120.0f,0.707,juce::Decibels::decibelsToGain(6.0f)); bbShelf_0W9Q.prepare(spec);
    // bitcrusher_AE6S no prepare
    // stereowidth_8H5C no prepare
}
void VelvetAudioProcessor::releaseResources() {}
bool VelvetAudioProcessor::isBusesLayoutSupported(const BusesLayout& l) const {
    return l.getMainOutputChannelSet()==juce::AudioChannelSet::stereo()&&l.getMainInputChannelSet()==juce::AudioChannelSet::stereo();
}
void VelvetAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    juce::ScopedNoDenormals noDenormals;
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    inputGainDsp.setGainDecibels(*apvts.getRawParameterValue("INPUT_GAIN")); inputGainDsp.process(context);
    gain_DJ2F.setGainDecibels(*apvts.getRawParameterValue("GAIN_GAIN_DB_DJ2F")); gain_DJ2F.process(context);
    comp_C87U.setThreshold(*apvts.getRawParameterValue("COMPRESSOR_THRESHOLD_C87U")); comp_C87U.setRatio(*apvts.getRawParameterValue("COMPRESSOR_RATIO_C87U")); comp_C87U.setAttack(*apvts.getRawParameterValue("COMPRESSOR_ATTACK_C87U")); comp_C87U.setRelease(*apvts.getRawParameterValue("COMPRESSOR_RELEASE_C87U")); compMakeup_C87U.setGainDecibels(*apvts.getRawParameterValue("COMPRESSOR_MAKEUP_C87U")); comp_C87U.process(context); compMakeup_C87U.process(context);
    { juce::dsp::Reverb::Parameters rp; rp.roomSize=*apvts.getRawParameterValue("REVERB_SIZE_B4W8")/100.0f; rp.damping=*apvts.getRawParameterValue("REVERB_DAMP_B4W8")/100.0f; float rvMix=*apvts.getRawParameterValue("REVERB_MIX_B4W8")/100.0f; rp.wetLevel=rvMix; rp.dryLevel=1.0f-rvMix; reverb_B4W8.setParameters(rp); reverb_B4W8.process(context); }
    { *bbShelf_0W9Q.state=*juce::dsp::IIR::Coefficients<float>::makeLowShelf(currentSampleRate,juce::jmax(20.0f,(float)*apvts.getRawParameterValue("BASSBOOSTER_FREQ_0W9Q")),0.707f,juce::Decibels::decibelsToGain((float)*apvts.getRawParameterValue("BASSBOOSTER_GAIN_0W9Q"))); bbShelf_0W9Q.process(context); float bbDrv=*apvts.getRawParameterValue("BASSBOOSTER_DRIVE_0W9Q")/100.0f*3.0f; if(bbDrv>0.01f){float bbS=1.0f+bbDrv; for(int ch=0;ch<buffer.getNumChannels();++ch){auto*d=buffer.getWritePointer(ch);for(int i=0;i<buffer.getNumSamples();++i){d[i]=std::tanh(d[i]*bbS)/bbS;}}} }
    { float bcBits=juce::jmax(2.0f,(float)*apvts.getRawParameterValue("BITCRUSHER_BITS_AE6S")); int bcRd=juce::jmax(1,(int)(float)*apvts.getRawParameterValue("BITCRUSHER_RATEDIV_AE6S")); float bcMix=*apvts.getRawParameterValue("BITCRUSHER_MIX_AE6S")/100.0f; float bcLvl=std::pow(2.0f,bcBits-1.0f); for(int ch=0;ch<buffer.getNumChannels();++ch){ auto*d=buffer.getWritePointer(ch); float held=0.0f; for(int i=0;i<buffer.getNumSamples();++i){ if(i%bcRd==0) held=std::round(d[i]*bcLvl)/bcLvl; d[i]=d[i]*(1.0f-bcMix)+held*bcMix; } } }
    { float swW=*apvts.getRawParameterValue("STEREOWIDTH_WIDTH_8H5C")/100.0f; if(block.getNumChannels()>=2){for(int i=0;i<(int)block.getNumSamples();++i){float swL=block.getSample(0,i),swR=block.getSample(1,i),swMid=(swL+swR)*.5f,swSide=(swL-swR)*.5f*swW;block.setSample(0,i,swMid+swSide);block.setSample(1,i,swMid-swSide);}} }
    outputGainDsp.setGainDecibels(*apvts.getRawParameterValue("OUTPUT_GAIN")); outputGainDsp.process(context);
}
void VelvetAudioProcessor::getStateInformation(juce::MemoryBlock& d) {
    auto s=apvts.copyState(); std::unique_ptr<juce::XmlElement> xml(s.createXml()); copyXmlToBinary(*xml,d);
}
void VelvetAudioProcessor::setStateInformation(const void* d,int sz) {
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(d,sz));
    if(xml&&xml->hasTagName(apvts.state.getType())) apvts.replaceState(juce::ValueTree::fromXml(*xml));
}
juce::AudioProcessorEditor* VelvetAudioProcessor::createEditor() { return new VelvetAudioProcessorEditor(*this); }
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new VelvetAudioProcessor(); }
