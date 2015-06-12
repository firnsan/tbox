#ifndef TBOX_LOGSTREAM_H
#define TBOX_LOGSTREAM_H

#include <string.h>

#include <string>

using std::string;


namespace tbox
{
	namespace detail
	{
		const int kSmallBuffer = 4000;

		template<int SIZE>
			class FixedBuffer
		{
		public:
		FixedBuffer()
			: cur_(data_)
			{

			}
			~FixedBuffer() {}

			void append(const char* buf, int len)
			{
				if (avail() > len) {
					memcpy(cur_, buf, len);
					cur_ += len;
				}
			}

			int avail() const { return static_cast<int>(end() - cur_); }
			const char *data() const { return data_; }
			int length() const { return static_cast<int>(cur_ - data_); }

			char *cur() { return cur_; }
			void add(size_t len) { cur_ += len; }

		private:
			const char *end() const { return data_ + sizeof(data_); }
			char data_[SIZE];
			char *cur_;
		};
	} //detail


	class LogStream {
		typedef LogStream self;
	public:
		typedef detail::FixedBuffer<detail::kSmallBuffer> Buffer; //还非得把kSmallBuffer声明为全局了？
		// LogStream(); 声明了就酸梅定义，也就没合成的了

		const Buffer &buffer() const {return buffer_;};

		self& operator<<(const string &str);
		self& operator<<(const char* str);
		self& operator<<(int num);
		self& operator<<(void *ptr);

	private:
		Buffer buffer_;

		static const int kMaxNumericSize = 32; //判断buffer_是否装得下一串数字
		//static const int kSmallBuffer = 4000; //类成员不可以当做模板变量？
	};

}
#endif
