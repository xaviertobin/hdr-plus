#include <Halide.h>

#include "align.h"
#include "merge.h"
#include "finish.h"

namespace {

    class StackFrames : public Halide::Generator<StackFrames> {
    public:
        // 'inputs' is really a series of raw 2d frames; extent[2] specifies the count
        Input<Halide::Buffer<uint16_t>> inputs{"inputs", 3};
        Input<uint16_t> black_point{"black_point"};
        Input<uint16_t> white_point{"white_point"};
        Input<float> white_balance_r{"white_balance_r"};
        Input<float> white_balance_g0{"white_balance_g0"};
        Input<float> white_balance_g1{"white_balance_g1"};
        Input<float> white_balance_b{"white_balance_b"};
        Input<int> cfa_pattern{"cfa_pattern"};
        Input<Halide::Buffer<float>> ccm{"ccm", 2}; // ccm - color correction matrix
        
        Input<float> compression{"compression"};
        Input<float> gain{"gain"};

        // Merged buffer
        Output<Halide::Buffer<uint16_t>> output{"output", 2};

        void generate() {
            Func alignment = align(inputs, inputs.width(), inputs.height());
            Func merged = merge(inputs, inputs.width(), inputs.height(), inputs.dim(2).extent(), alignment);

            CompiletimeWhiteBalance2 wb{ white_balance_r, white_balance_g0, white_balance_g1, white_balance_b };
            Func fixed = touchup(merged, inputs.width(), inputs.height(), black_point, white_point, wb, cfa_pattern, ccm, compression, gain);

            output = fixed;
        }
    };

}  // namespace

HALIDE_REGISTER_GENERATOR(StackFrames, align_and_merge)
