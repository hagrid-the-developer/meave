#ifndef MEAVE_CRC_TEST_H
#	define MEAVE_CRC_TEST_H

#	include <functional>

namespace meave { namespace crc { namespace test {

typedef std::function<::uint32_t(const ::uint8_t*, const ::size_t)> CRCFunc;

void compare_output(const char *name, CRCFunc f_expected, CRCFunc f_real);
void measure_speed(const char *name, CRCFunc f);

} } } /* meave::crc::test */

#endif // MEAVE_CRC_TEST_H
