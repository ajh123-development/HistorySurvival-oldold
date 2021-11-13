#pragma once
#include <net_common.hpp>

#ifndef NET_MESSAGE_H
#define NET_MESSAGE_H

namespace hsc {
	namespace net {
		namespace packets {
			//Message header this is sent at the start of every message. The  
			//templates allows us to use `enum class` to make sure all
			//messages are valid at compile time.
			template <typename T>
			struct message_header
			{
				T id{};
				uint32_t size = 0;
			};

			//Messages contain a body made of bytes and a header at the
			//start, they can be serialised and deserialsised. by using
			//special functions.
			template <typename T>
			struct message
			{
				message_header<T> header{};
				std::vector<uint8_t> body;

				//Return the size of the whole packet in bytes
				size_t size()  const {
					return sizeof(message_header<T>) + body.size();
				}

				//Overide for bit sift left `<<`, allows us to use `std::cout`
				//for message debuging - as an example.
				friend std::ostream& operator << (std::ostream& os, const message<T>& msg) {
					os << "id:" << int(msg.header.id) << "\nsize:" << msg.header.size;
					return os;
				}

				//Overide for bit sift left `<<`, allows us to use our
				//message to push on to, like a stack.
				template <typename DataType>
				friend message<T>& operator << (message<T>& msg, const DataType& data) {
					//Check that data is 'copyable'
					static_assert(std::is_standard_layout<DataType>::value, "Data is to complex to push!");

					//Cache the current size
					size_t size = msg.body.size();
					//Resize the vector so data can be pushed
					msg.body.resize(msg.body.size() + sizeof(DataType));
					//Copy the new data into the free space
					std::memcpy(msg.body.data() + size, &data, sizeof(DataType));
					//Update the size in message's header
					msg.header.size = msg.size();

					//Return the message so we can chain pushes
					return msg;
				}

				//Overide for bit sift left `>>`, allows us to use our
				//message to pop off, like a stack.
				template <typename DataType>
				friend message<T>& operator >> (message<T>& msg, const DataType& data) {
					//Check that data is 'copyable'
					static_assert(std::is_standard_layout<DataType>::value, "Data is to complex to poped!");

					//Cache the current size, to where data will be poped from
					size_t size = msg.body.size() - sizeof(DataType);
					//Copy the poped data into the data
					std::memcpy(&data, msg.body.data() + size, sizeof(DataType));
					//Update the size in message's header
					msg.body.resize(size);
					msg.header.size = msg.size();

					//Return the message so we can chain pushes
					return msg;
				}
			};
		}
	}
}

#endif