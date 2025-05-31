/*
 *  This Source Code Form is subject to the terms of the Mozilla Public License,
 *  v. 2.0. If a copy of the MPL was not distributed with this file, You can
 *  obtain one at http://mozilla.org/MPL/2.0/.
 *
 *  The original code is copyright (c) 2022, open.mp team and contributors.
 */

// Required for most of open.mp.
#include <sdk.hpp>

#include <iostream>
#include <vector>
#include <iomanip>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/crypto.h>

class SSLCompTest final : public IComponent, public PoolEventHandler<IVehicle>
{
private:
	ICore* core_ = nullptr;
	
public:
	PROVIDE_UID(0x5455F37DF06421A5);

	~SSLCompTest()
	{
	}
	
	StringView componentName() const override
	{
		return "SSL Component Test";
	}

	SemanticVersion componentVersion() const override
	{
		return SemanticVersion(0, 0, 1, 0);
	}

	bool cryptoRandom(void* buffer, size_t length)
	{
		unsigned char* buf = static_cast<unsigned char*>(buffer);
		do
		{
			if (1 == RAND_status())
			{
				if (1 == RAND_bytes_ex(nullptr, buf, length, 0))
					return true;
			}

			const auto code = ERR_peek_last_error();
			if (ERR_GET_LIB(code) == ERR_LIB_RAND)
			{
				const auto reason = ERR_GET_REASON(code);
				if (reason == RAND_R_ERROR_INSTANTIATING_DRBG || reason == RAND_R_UNABLE_TO_FETCH_DRBG || reason == RAND_R_UNABLE_TO_CREATE_DRBG)
				{
					return false;
				}
			}
		} while (1 == RAND_poll());

		return false;
	}

	void onLoad(ICore* c) override
	{
		// Cache core, player pool here
		core_ = c;
		core_->printLn("SSL Component Test template loaded.");

		// Initialize OpenSSL
		OPENSSL_init_crypto(OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);

		if (cryptoRandom(nullptr, 0))
		{
			std::cout << "works\n";
		}
		else
		{
			std::cout << "doesn't work aaa\n";
		}

		const int num_bytes = 32; // 256 bits
		std::vector<unsigned char> random_bytes(num_bytes);

		if (RAND_bytes(random_bytes.data(), num_bytes) != 1)
		{
			std::cerr << "Failed to generate random bytes: ";
			ERR_print_errors_fp(stderr);
			return;
		}

		std::cout << "Generated " << num_bytes << " random bytes: ";
		for (unsigned char byte : random_bytes)
		{
			std::cout << std::hex << std::setw(2) << std::setfill('0')
					  << static_cast<int>(byte);
		}
		std::cout << std::endl;

		OPENSSL_cleanup();
	}

	void onInit(IComponentList* components) override
	{
	}

	void onReady() override
	{
	}

	void onFree(IComponent* component) override
	{
	}

	void free() override
	{
		delete this;
	}

	void reset() override
	{
	}
};

// Automatically called when the compiled binary is loaded.
COMPONENT_ENTRY_POINT()
{
	return new SSLCompTest();
}
