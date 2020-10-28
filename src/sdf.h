#pragma once

#include <sstream>
#include <tiffio.h>
#include <tiffio.hxx>

#include <HalideBuffer.h>

class RawImage;

class LibRaw2DngConverter {
    using TiffPtr = std::shared_ptr<TIFF>;
    TiffPtr SetTiffFields(TiffPtr tiff_ptr);
public:
    explicit LibRaw2DngConverter(const RawImage& raw, const std::string sampleFile);

    void SetBuffer(const Halide::Runtime::Buffer<uint16_t>& buffer) const;

    void Write(const std::string& path) const;

private:
    std::ostringstream OutputStream;
    const RawImage& Raw;
    const std::string input_file;
    std::shared_ptr<TIFF> Tiff;
};
