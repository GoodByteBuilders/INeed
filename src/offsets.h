#pragma once
#include <cstddef>

namespace offsets {
	namespace client_dll {
		constexpr std::ptrdiff_t dwEntityList = 0x1A292F0;
		constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x187CEF0;

		constexpr std::ptrdiff_t m_iHealth = 0x344; // int32
		constexpr std::ptrdiff_t m_iIDEntIndex = 0x1458; // CEntityIndex
		constexpr std::ptrdiff_t m_iTeamNum = 0x3E3; // uint8
		constexpr std::ptrdiff_t m_hPawn = 0x62C; // CHandle<C_BasePlayerPawn>
	}

	namespace buttons {
		constexpr std::ptrdiff_t attack = 0x1875710;
		constexpr std::ptrdiff_t jump = 0x1875C20;
		constexpr std::ptrdiff_t reload = 0x1875680;
	}
}