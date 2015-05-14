#ifndef TBOX_NONCOPYABLE_H
#define TBOX_NONCOPYABLE_H

class NonCopyable
{
	NonCopyable(const NonCopyable&) = delete;
	const NonCopyable &operator=(const NonCopyable&) = delete;

protected:
	NonCopyable() = default;
	~NonCopyable() = default;

};

#endif
