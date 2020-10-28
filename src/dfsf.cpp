#include "LibRaw2DngConverter.h"

#include <unordered_map>
#include <array>

#include <libraw/libraw.h>

#include "InputSource.h"

LibRaw2DngConverter::LibRaw2DngConverter(const RawImage &raw, const std::string sampleFile)
    : OutputStream(), Raw(raw), Tiff(SetTiffFields(TiffPtr(TIFFStreamOpen("", &OutputStream), TIFFClose))),
      input_file(sampleFile)
{
}

LibRaw2DngConverter::TiffPtr LibRaw2DngConverter::SetTiffFields(LibRaw2DngConverter::TiffPtr tiff_ptr)
{

    // TIFF* file;
    // file = TIFFOpen(input_file.c_str(), "rh");
    // std::cerr << "Successfully opened base DNG" << std::endl;
    // std::array<float, 9> *data;
    // if (file != NULL) {
    //     std::cerr << "Successfully opened base DNG" << std::endl;
    //     uint16 count;
    //     TIFFGetField(file, TIFFTAG_COLORMATRIX1, data, count);
    //     printf("%f\n", data->at(0));
    //     // std::cerr << "data at 0 = " + data << std::endl;
    //     TIFFClose(file);
    // }

    const auto RawProcessor = Raw.GetRawProcessor();
    const auto raw_color = RawProcessor->imgdata.color;

    const uint16_t bayer_pattern_dimensions[] = {2, 2};

    const auto tiff = tiff_ptr.get();

    // CORE TAGS
    TIFFSetField(tiff, TIFFTAG_DNGVERSION, "\01\04\00\00");
    TIFFSetField(tiff, TIFFTAG_DNGBACKWARDVERSION, "\01\04\00\00");
    TIFFSetField(tiff, TIFFTAG_SUBFILETYPE, 0);
    TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, 16);
    TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, 1);
    TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_CFA);
    TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 1);
    TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tiff, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
    TIFFSetField(tiff, TIFFTAG_CFAREPEATPATTERNDIM, &bayer_pattern_dimensions);
    const std::string cfa = Raw.GetCfaPatternString();
    TIFFSetField(tiff, TIFFTAG_CFAPATTERN, cfa.c_str());

    // DESCRIPTIVE TAGS
    TIFFSetField(tiff, TIFFTAG_MAKE, "hdr-plus");
    TIFFSetField(tiff, TIFFTAG_UNIQUECAMERAMODEL, "hdr-plus");

    // COLOUR MATRIX TAGS
    const std::array<float, 9> color_matrix = {
        0.6502f,
        -0.2407f,
        0.0023f,
        -0.3899f,
        1.1688f,
        0.2511f,
        -0.0469f,
        0.1068f,
        0.6781f,
    };
    const std::array<float, 9> color_matrix2 = {
        0.5973f,
        -0.1695f,
        -0.0419f,
        -0.3826f,
        1.1797f,
        0.2293f,
        -0.0639f,
        0.1398f,
        0.5789f,
    };

    TIFFSetField(tiff, TIFFTAG_COLORMATRIX1, 9, &color_matrix);
    TIFFSetField(tiff, TIFFTAG_COLORMATRIX2, 9, &color_matrix2);

    // COLOUR CALIBRATION TAGS TAGS
    const std::array<float, 9> color_calib = {
        1.f,
        0.f,
        0.f,
        0.f,
        1.f,
        0.f,
        0.f,
        0.f,
        1.0068f,
    };
    const std::array<float, 9> color_calib2 = {
        1.f,
        0.f,
        0.f,
        0.f,
        1.f,
        0.f,
        0.f,
        0.f,
        1.0068f,
    };
    TIFFSetField(tiff, TIFFTAG_CAMERACALIBRATION1, 9, &color_calib);
    TIFFSetField(tiff, TIFFTAG_CAMERACALIBRATION2, 9, &color_calib2);

    // Default balance
    const std::array<float, 9> analog_balance = {
        1.f,
        1.f,
        1.f,
    };
    TIFFSetField(tiff, TIFFTAG_ANALOGBALANCE, 9, &analog_balance);

    // Default balance
    const std::array<float, 3> as_shot_neutral = {
        0.346883f, 1.f, 0.593968f};

    TIFFSetField(tiff, TIFFTAG_ASSHOTNEUTRAL, 3, &as_shot_neutral);

     TIFFSetField(tiff, TIFFTAG_CALIBRATIONILLUMINANT1, 17); // D65

    // const std::array<float, 3> as_shot_neutral = { 2.88, 1.0, 1.67};

    TIFFSetField(tiff, TIFFTAG_CFALAYOUT, 1);                  // Rectangular (or square) layout
    TIFFSetField(tiff, TIFFTAG_CFAPLANECOLOR, 3, "\00\01\02"); // RGB https://www.awaresystems.be/imaging/tiff/tifftags/cfaplanecolor.html

    const std::array<float, 4> black_level = Raw.GetBlackLevel();
    TIFFSetField(tiff, TIFFTAG_BLACKLEVEL, 4, &black_level);

    static const uint32_t white_level = raw_color.maximum * 4;
    TIFFSetField(tiff, TIFFTAG_WHITELEVEL, 1, &white_level );

    if (RawProcessor->imgdata.sizes.flip > 0)
    {
        // Seems that LibRaw uses LibTIFF notation.
        TIFFSetField(tiff, TIFFTAG_ORIENTATION, RawProcessor->imgdata.sizes.flip);
    }
    else
    {
        TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    }
    return tiff_ptr;
}

void LibRaw2DngConverter::SetBuffer(const Halide::Runtime::Buffer<uint16_t> &buffer) const
{
    const auto width = buffer.width();
    const auto height = buffer.height();
    const auto tiff = Tiff.get();
    TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, height);

    uint16_t *row_pointer = buffer.data();
    for (int row = 0; row < height; row++)
    {
        TIFFWriteScanline(tiff, row_pointer, row, 0);
        row_pointer += width;
    }
}

void LibRaw2DngConverter::Write(const std::string &path) const
{
    TIFFCheckpointDirectory(Tiff.get());
    TIFFFlush(Tiff.get());
    std::ofstream output(path, std::ofstream::binary);
    output << OutputStream.str();
}
