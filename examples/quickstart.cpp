/*
 * -----------------------------------------------------------------------------
 * Proje: tcknvkn-cpp
 * Dosya: examples/quickstart.cpp
 * Açıklama: TCKN ve VKN doğrulama fonksiyonlarının hızlı kullanım örneğini içerir.
 * Oluşturma Tarihi: 2026-04-24
 * Lisans: MIT
 * Site: https://www.tcknvkn.com
 * -----------------------------------------------------------------------------
 */

#include "tcknvkn.hpp"

#include <iostream>

int main() {
    const auto tckn = tcknvkn::validate_tckn("10000000146");
    const auto vkn = tcknvkn::validate_vkn("1000036109");

    std::cout << "TCKN valid: " << tckn.valid << '\n';
    std::cout << "VKN valid: " << vkn.valid << '\n';
    return 0;
}
