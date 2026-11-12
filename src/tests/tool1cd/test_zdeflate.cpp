/*
    test_project provides tests for Tool1CD library
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of test_project.

    test_project is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    test_project is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with test_project.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "../catch.hpp"
#include <UZLib.h>
#include <SystemClasses/TMemoryStream.hpp>
#include <zlib.h>

#include <algorithm>
#include <cstdint>
#include <vector>

using namespace System;

// Размер входного буфера ZDeflateStream (CHUNKSIZE в UZLib.cpp).
// Если исходник ровно N таких блоков, следующий Read возвращает 0.
// Это обычный EOF: raw-deflate нужно закрыть Z_FINISH, иначе в cf
// уезжает обрезанный поток (inflate не доходит до Z_STREAM_END).
static const int64_t DEFLATE_CHUNK = 16384;

static std::vector<uint8_t> payload(size_t size)
{
	std::vector<uint8_t> data(size);
	for (size_t i = 0; i < size; ++i) {
		data[i] = static_cast<uint8_t>(i * 131u + 17u);
	}
	return data;
}

// raw inflate, как у ZInflateStream (windowBits = -MAX_WBITS).
// eof == true только если zlib дошёл до Z_STREAM_END.
static bool raw_inflate(const std::vector<uint8_t> &src, std::vector<uint8_t> &dst)
{
	dst.clear();
	if (src.empty()) {
		return false;
	}

	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	if (inflateInit2(&strm, -MAX_WBITS) != Z_OK) {
		return false;
	}

	const size_t chunk = static_cast<size_t>(DEFLATE_CHUNK);
	std::vector<unsigned char> out(chunk);
	size_t offset = 0;
	int ret = Z_OK;

	do {
		size_t n = std::min(chunk, src.size() - offset);
		strm.avail_in = static_cast<unsigned int>(n);
		strm.next_in = const_cast<unsigned char *>(src.data() + offset);
		offset += n;

		do {
			strm.avail_out = static_cast<unsigned int>(chunk);
			strm.next_out = out.data();
			ret = inflate(&strm, Z_NO_FLUSH);
			if (ret != Z_OK && ret != Z_STREAM_END) {
				inflateEnd(&strm);
				return false;
			}
			size_t have = chunk - strm.avail_out;
			dst.insert(dst.end(), out.begin(), out.begin() + static_cast<std::ptrdiff_t>(have));
		} while (strm.avail_out == 0);
	} while (ret != Z_STREAM_END && offset < src.size());

	inflateEnd(&strm);
	return ret == Z_STREAM_END;
}

static void expect_roundtrip(int64_t size)
{
	INFO("uncompressed size = " << size);

	auto original = payload(static_cast<size_t>(size));
	TMemoryStream src;
	if (!original.empty()) {
		src.Write(original.data(), static_cast<int64_t>(original.size()));
		src.SetPosition(0);
	}

	TMemoryStream packed;
	bool finished = ZDeflateStream(&src, &packed);
	CHECK(finished);
	if (!finished) {
		return;
	}

	std::vector<uint8_t> inflated;
	bool eof = raw_inflate(packed.GetBytes(), inflated);
	CHECK(eof);
	if (!eof) {
		return;
	}
	CHECK(inflated.size() == original.size());
	if (inflated.size() != original.size()) {
		return;
	}
	CHECK(std::equal(inflated.begin(), inflated.end(), original.begin()));

	packed.SetPosition(0);
	TMemoryStream unpacked;
	CHECK_NOTHROW(ZInflateStream(&packed, &unpacked));
	const auto &via_stream = unpacked.GetBytes();
	CHECK(via_stream.size() == original.size());
	if (via_stream.size() != original.size()) {
		return;
	}
	CHECK(std::equal(via_stream.begin(), via_stream.end(), original.begin()));
}

TEST_CASE("ZDeflateStream закрывает raw-deflate на границе блока", "[tool1cd][UZLib][ZDeflateStream]")
{
	// Короче блока и на байт длиннее: Z_FINISH и так выставляется по avail_in < CHUNKSIZE.
	expect_roundtrip(0);
	expect_roundtrip(1);
	expect_roundtrip(DEFLATE_CHUNK - 1);
	expect_roundtrip(DEFLATE_CHUNK + 1);

	// Длина TOC versions N×16384. После полного блока Read == 0, без Z_FINISH
	// поток обрывается: inflate даёт eof=false и короче исходника.
	expect_roundtrip(DEFLATE_CHUNK);
	expect_roundtrip(DEFLATE_CHUNK * 2);
}
