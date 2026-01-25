#include "ByteVectorUtils.hpp"

namespace ByteVectorUtils
{

NO_DISCARD ByteVector uint32_to_byte_vector(const uint32_t in_uint)
{
	ByteVector out_vector;
	out_vector.resize(sizeof(uint32_t));

	std::memcpy(out_vector.data(), &in_uint, sizeof(uint32_t));

	return out_vector;
}

}