/*
   ==============================================================================

    OversampledClipper.h
    Created: 3 Feb 2021 1:03:59pm
    Author:  Vladyslav Voinov

   ==============================================================================
 */
#pragma once

#include <cmath>

#include <juce_dsp/juce_dsp.h>

#include "Clipper.h"

// ==============================================================================
namespace pe
{
namespace dsp
{
    namespace waveshaping
    {
        // ==============================================================================
        template <typename T>
        class OversampledClipper : public juce::dsp::ProcessorBase
        {
        public:
            //==============================================================================
            /** Aliases */
            using Oversampling = juce::dsp::Oversampling<T>;
            using ProcessorSpec = juce::dsp::ProcessSpec;

            //==============================================================================
            OversampledClipper (const unsigned int factor = 1) noexcept
                : oversampler (2, factor, Oversampling::filterHalfBandPolyphaseIIR, false)
            {
            }

            ~OversampledClipper() override
            {
            }

            //==============================================================================
            /** Inheritet implemented methods */
            void prepare (const ProcessorSpec& spec) noexcept override
            {
                /** Setup oversampler */
                oversampler.reset();
                oversampler.initProcessing (spec.maximumBlockSize);

                const auto oversampledSpec = createOversampledSpec (spec);

                /** Setup pre-filter*/
                *preFilter.state = *juce::dsp::IIR::Coefficients<T>::makeLowPass (oversampledSpec.sampleRate, calculateCutoff (oversampledSpec.sampleRate));
                preFilter.prepare (oversampledSpec);

                /** Setup clipper*/
                clipper.prepare (oversampledSpec);
                setCeiling (DEFAULT_CEILING);
                setClippingType (DEFAULT_CLIPPING_TYPE);

                /** Setup post-filter*/
                *postFilter.state = *juce::dsp::IIR::Coefficients<T>::makeLowPass (oversampledSpec.sampleRate, calculateCutoff (oversampledSpec.sampleRate));
                postFilter.prepare (oversampledSpec);
            }

            void reset() noexcept override
            {
                oversampler.reset();
                preFilter.reset();
                clipper.reset();
                postFilter.reset();
            }

            void process (const juce::dsp::ProcessContextReplacing<T>& context) noexcept override
            {
                auto oversampledAudioBlock = oversampler.processSamplesUp (context.getInputBlock());
                auto oversampledContext = juce::dsp::ProcessContextReplacing<T> (oversampledAudioBlock);

                preFilter.process (oversampledContext);
                clipper.process (oversampledContext);
                postFilter.process (oversampledContext);

                oversampler.processSamplesDown (context.getOutputBlock());
            }

            // ==============================================================================
            /** Public interface */
            void setCeiling (float ceilingDbValue) noexcept
            {
                clipper.setThreshold (ceilingDbValue);
            }

            void setClippingType (ClippingType clippingType) noexcept
            {
                clipper.setClippingType (clippingType);
            }

        private:
            //==============================================================================
            /** Default values */
            const float DEFAULT_FILTER_Q_OCTAVES = 2.0f;
            const float DEFAULT_CEILING = 0.0f;
            const ClippingType DEFAULT_CLIPPING_TYPE = ClippingType::HARD;

            //==============================================================================
            /** DSP */
            Oversampling oversampler;
            Clipper<T> clipper;
            juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<T>, juce::dsp::IIR::Coefficients<T>> preFilter;
            juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<T>, juce::dsp::IIR::Coefficients<T>> postFilter;

            //==============================================================================
            /** Filter calculations based on input params */
            unsigned int calculateCutoff (double sampleRate) const noexcept
            {
                return static_cast<unsigned int> ((sampleRate / 2) * 0.98f);
            }

            double calculateQ (float octaves) const noexcept
            {
                jassert (octaves >= 0.0f & octaves <= 4); // maximum 4 octaves allowed, no negative octaves

                return 1.0f / sqrtf (octaves);
            }

            //==============================================================================
            /** Helper builders */
            const ProcessorSpec createOversampledSpec (ProcessorSpec const& src) const noexcept
            {
                const unsigned int xOversample = static_cast<unsigned int> (sqrt (oversampler.getOversamplingFactor()));
                return { src.sampleRate * xOversample, src.maximumBlockSize * xOversample, src.numChannels };
            }

            //==============================================================================
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OversampledClipper)
        };
    } // namespace waveshaping
} // namespace dsp
} // namespace pe
