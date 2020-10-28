#include <stdio.h>
#include <iostream>
#include <fstream>

#include <Halide.h>

#include <src/Burst.h>

#include <align_and_merge.h>

int main(int argc, char *argv[])
{

    Compression c = 2.f;
    Gain g = 1.f;

    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " dir_path out_img raw_img1 raw_img2 [...]" << std::endl;
        return 1;
    }

    int i = 1;
    if (argc - i < 3)
    {
        std::cerr << "Usage: " << argv[0] << " dir_path out_img raw_img1 raw_img2 [...]" << std::endl;
        return 1;
    }

    const std::string dir_path = argv[i++];
    const std::string out_name = argv[i++];

    std::vector<std::string> in_names;
    while (i < argc)
        in_names.push_back(argv[i++]);

    Burst burst(dir_path, in_names);

    // Gather the secondary data required for processing
    const int width = burst.GetWidth();
    const int height = burst.GetHeight();
    const WhiteBalance wb = burst.GetWhiteBalance();
    const int cfa_pattern = static_cast<int>(burst.GetCfaPattern());
    auto ccm = burst.GetColorCorrectionMatrix();

    Halide::Runtime::Buffer<uint16_t> output_img(width, height);

    align_and_merge(burst.ToBuffer(), burst.GetBlackLevel(), burst.GetWhiteLevel(), wb.r, wb.g0, wb.g1, wb.b, cfa_pattern, ccm, c, g, output_img);
    std::cerr << "merged size: " << output_img.width() << " " << output_img.height() << std::endl;

    const RawImage &raw = burst.GetRaw(0);
    const std::string merged_filename = dir_path + "/" + out_name;
    raw.WriteDng(merged_filename, dir_path + "/" + in_names[0], output_img);

    return EXIT_SUCCESS;
}
