#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>


class TransposeView
{
public:
	TransposeView(uint8_t* image, size_t width, size_t height)
		: mImage(image)
		, mPos(0)
		, mSize(width* height)
		, mWidth(height)
		, mHeight(width)
		, mBlockWidth(mWidth)
		, mBlockHeight(mHeight)
	{}

	TransposeView(uint8_t* image, size_t width, size_t height, size_t blockWidth, size_t blockHeight)
		: mImage(image)
		, mSize(width* height)
		, mWidth(height)
		, mHeight(width)
		, mBlockWidth(blockHeight)
		, mBlockHeight(blockWidth)
	{
		mPos = mWidth * (mHeight - 1);
	}

	uint8_t* begin() { return mImage + mPos; }
	uint8_t* end() { return mImage + mSize; }

	TransposeView& operator++()
	{
		auto _remainder = mPos % (mWidth * mBlockHeight);

		if (_remainder < mWidth) //need jump
		{
			if ((_remainder + 1) % mBlockWidth == 0) //block jump
			{
				if (mPos < mWidth) //block shift down-right
				{
					mPos += ((mHeight - 1) * mWidth + 1);
				}
				else//block shift up
				{
					mPos -= (mWidth + mBlockWidth - 1);
				}

			}
			else //jump same block next row
			{
				auto next_row = ((mBlockHeight - 1) * mWidth) + 1;
				mPos += next_row;
			}
		}
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
		return *(mImage+ mPos);
	}

	void push_back(uint8_t val)
	{
		*(mImage + mPos) = val;
		++(*this);
	}
private:
	uint8_t*	mImage;
	size_t		mPos;
	size_t      mSize;
	size_t      mWidth;
	size_t      mHeight;
	size_t		mBlockWidth;
	size_t		mBlockHeight;
};

std::vector<uint16_t> vec1{	00, 00, 0	,0	,0	,0	,0	,0	, 00, 00,
							00, 00, 1	,2	,3	,4	,5	,6	, 00, 00,
							00, 00, 11	,12	,13	,14	,15	,16	, 00, 00,
							00, 00, 21	,22	,23	,24	,25	,26	, 00, 00,
							00, 00, 31	,32	,33	,34	,35	,36	, 00, 00,
							00, 00, 0	,0	,0	,0	,0	,0	, 00, 00};

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
	size_t block_width{ 3 }, block_height{ 2 };
	TransposeView tran_view{output, src_cropped_width, src_cropped_height, block_width, block_height};
	for (auto& entry : vec1)
	{
		std::cout << entry << '\n';

	}

	std::cout << "===================\n";
	for (size_t x = 0; x < src_cropped_height; x += block_height)
	{
		for (size_t y = 0; y < src_cropped_width; y += block_width)
		{
			auto init_offset = src_width + 2;
			auto total_offset = init_offset + x * src_width + y;
			//std::cout << std::hex << vec1[total_offset] << '\n';

			for (size_t i = 0; i < block_height; ++i)
			{
				auto offset = src_width + 2 + i * src_width;
				std::transform(vec1.begin() + total_offset + i * src_width, vec1.begin() + total_offset + i * src_width + block_width, std::back_inserter(tran_view),
					[](auto val)
					{
						return static_cast<uint8_t>(val);
					});
			}
		}
	}


	for (int i = 0; i < 24; ++i)
	{
		std::cout << std::hex << static_cast<int16_t>(output[i]) << '\n';

	}

	std::cin.get();
	return 0;
}
