#include <vector>
#include <algorithm>
#include <iostream>

class TransposeView
{
public:
	TransposeView(uint8_t* image, size_t width, size_t height)
		: mImage(image)
		, mSize(width* height)
		, mWidth(height)
		, mHeight(width)
	{
		mPos = begin();
	}

	uint8_t* begin() { return mImage + mWidth * (mHeight - 1); }
	uint8_t* end() { return mImage + mSize; }

	TransposeView& operator++()
	{
		if ((mPos - mWidth) < mImage)
			mPos = mPos + mWidth * (mHeight - 1) + 1;
		else
			mPos -= mWidth;
		return *this;
	}

	size_t width() const { return mWidth; }
	size_t height() const { return mHeight; }

	TransposeView& operator+(size_t i)
	{
		for (; i > 0; --i)
		{		
			++(*this);
		}
		return *this;
	}

	uint8_t& operator*()
	{
		return *mPos;
	}

private:
	uint8_t*	mImage;
	uint8_t*	mPos;
	size_t      mSize;
	size_t      mWidth;
	size_t      mHeight;
};

std::vector<uint16_t> vec1{	00, 00, 0x00	,0x00	,0x00	,0x00	,0x00	,0x00	, 00, 00,
							00, 00, 0x11	,0x12	,0x13	,0x14	,0x15	,0x16	, 00, 00,
							00, 00, 0x21	,0x22	,0x23	,0x24	,0x25	,0x26	, 00, 00,
							00, 00, 0x31	,0x32	,0x33	,0x34	,0x35	,0x36	, 00, 00,
							00, 00, 0x41	,0x42	,0x43	,0x44	,0x45	,0x46	, 00, 00,
							00, 00, 0x00	,0x00	,0x00	,0x00	,0x00	,0x00	, 00, 00};

/*std::vector<uint16_t> vec1{00, 00, 0x11	,0x12	,0x13	,0x14	,0x15	,0x16	, 00, 00,
							00, 00, 0x21	,0x22	,0x23	,0x24	,0x25	,0x26	, 00, 00,
							00, 00, 0x31	,0x32	,0x33	,0x34	,0x35	,0x36	, 00, 00,
							00, 00, 0x41	,0x42	,0x43	,0x44	,0x45	,0x46	, 00, 00 };*/

std::vector<uint16_t> vec2{ 11	,12	,13	,14	,15	,16	,
							21	,22	,23	,24	,25	,26	,
							31	,32	,33	,34	,35	,36	,
							41	,42	,43	,44	,45	,46 };
uint8_t* output = new uint8_t(24);

int main()
{
	size_t src_width = 10;
	size_t src_cropped_width{ 6 }, src_cropped_height{ 4 };
	TransposeView tran_view{output, src_cropped_width, src_cropped_height };
	for (auto& entry : vec1)
	{
		std::cout << entry << '\n';

	}


	std::cout << "===================\n";
	for (size_t i = 0; i < src_cropped_height; ++i)
	{
		auto mul = static_cast<int>(i && 1);
		std::transform(vec1.begin() + src_width + 2 + i* src_width, vec1.begin() + src_width + 2 + src_cropped_width + i* src_width, tran_view + mul*tran_view.height(),
			[](auto val)
			{
				return val;
			});
	}
	
	for (int i = 0; i < 24; ++i)
	{
		std::cout << static_cast<int16_t>(output[i]) << '\n';

	}

	std::cin.get();
	return 0;
}
