/*
 * -----------------------------------------------------------------------------
 * Proje: tcknvkn-cpp
 * Dosya: tests/test_tcknvkn.cpp
 * Açıklama: TCKN ve VKN doğrulama fonksiyonları için varyasyonlu testleri içerir.
 * Oluşturma Tarihi: 2026-04-24
 * Lisans: MIT
 * Site: https://www.tcknvkn.com
 * -----------------------------------------------------------------------------
 */

#include "tcknvkn.hpp"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace {

/* contains_error hata listesinde beklenen hata metnini arar. */
bool contains_error(const tcknvkn::ValidationResult &result, const std::string &target) {
    for (const auto &err : result.errors) {
        if (err == target) {
            return true;
        }
    }
    return false;
}

/* test_validate_tckn_valid geçerli bir TCKN doğrulamasını test eder. */
void test_validate_tckn_valid() {
    const auto result = tcknvkn::validate_tckn("10000000146");
    assert(result.valid);
    assert(result.value == "10000000146");
    assert(result.errors.empty());
}

/* test_validate_tckn_normalize normalize edilmiş TCKN girdisini test eder. */
void test_validate_tckn_normalize() {
    const auto result = tcknvkn::validate_tckn("100-000 00146");
    assert(result.valid);
    assert(result.value == "10000000146");
}

/* test_validate_tckn_invalid_length uzunluğu hatalı TCKN girdisini test eder. */
void test_validate_tckn_invalid_length() {
    const auto result = tcknvkn::validate_tckn("12345");
    assert(!result.valid);
    assert(contains_error(result, "11 haneli olmalıdır."));
}

/* test_validate_tckn_leading_zero ilk hanesi sıfır olan TCKN girdisini test eder. */
void test_validate_tckn_leading_zero() {
    const auto result = tcknvkn::validate_tckn("01234567890");
    assert(!result.valid);
    assert(contains_error(result, "İlk hane 0 olamaz."));
}

/* test_validate_tckn_checksum checksum hatalı TCKN girdisini test eder. */
void test_validate_tckn_checksum() {
    const auto result = tcknvkn::validate_tckn("10000000145");
    assert(!result.valid);
    assert(contains_error(result, "11. hane kontrol hanesi hatalı."));
}

/* test_validate_tckn_same_digits tüm haneleri aynı TCKN girdisini test eder. */
void test_validate_tckn_same_digits() {
    const auto result = tcknvkn::validate_tckn("11111111111");
    assert(!result.valid);
    assert(contains_error(result, "Geçersiz örüntü: tüm haneler aynı."));
}

/* test_validate_tckn_multiple_errors çoklu hata üretimini test eder. */
void test_validate_tckn_multiple_errors() {
    const auto result = tcknvkn::validate_tckn("0");
    assert(!result.valid);
    assert(contains_error(result, "11 haneli olmalıdır."));
    assert(contains_error(result, "İlk hane 0 olamaz."));
}

/* test_validate_vkn_valid geçerli bir VKN doğrulamasını test eder. */
void test_validate_vkn_valid() {
    const auto result = tcknvkn::validate_vkn("1000036109");
    assert(result.valid);
    assert(result.value == "1000036109");
    assert(result.errors.empty());
}

/* test_validate_vkn_normalize normalize edilmiş VKN girdisini test eder. */
void test_validate_vkn_normalize() {
    const auto result = tcknvkn::validate_vkn("100-003-6109");
    assert(result.valid);
    assert(result.value == "1000036109");
}

/* test_validate_vkn_invalid_length uzunluğu hatalı VKN girdisini test eder. */
void test_validate_vkn_invalid_length() {
    const auto result = tcknvkn::validate_vkn("1234");
    assert(!result.valid);
    assert(contains_error(result, "10 haneli olmalıdır."));
}

/* test_validate_vkn_checksum checksum hatalı VKN girdisini test eder. */
void test_validate_vkn_checksum() {
    const auto result = tcknvkn::validate_vkn("1000036108");
    assert(!result.valid);
    assert(contains_error(result, "Son hane kontrol hanesi hatalı."));
}

/* test_validate_vkn_same_digits tüm haneleri aynı VKN girdisini test eder. */
void test_validate_vkn_same_digits() {
    const auto result = tcknvkn::validate_vkn("1111111111");
    assert(!result.valid);
    assert(contains_error(result, "Geçersiz örüntü: tüm haneler aynı."));
}

/* test_validate_multiple_tckn çoklu TCKN doğrulamasını test eder. */
void test_validate_multiple_tckn() {
    const std::vector<std::string> inputs{"10000000146", "10000000145", "11111111111"};
    const auto results = tcknvkn::validate_multiple_tckn(inputs);
    assert(results.size() == 3);
    assert(results[0].valid);
    assert(!results[1].valid);
    assert(!results[2].valid);
}

/* test_validate_multiple_tckn_empty boş TCKN listesi davranışını test eder. */
void test_validate_multiple_tckn_empty() {
    const std::vector<std::string> inputs{};
    const auto results = tcknvkn::validate_multiple_tckn(inputs);
    assert(results.empty());
}

/* test_validate_multiple_vkn çoklu VKN doğrulamasını test eder. */
void test_validate_multiple_vkn() {
    const std::vector<std::string> inputs{"1000036109", "1000036108", "1111111111"};
    const auto results = tcknvkn::validate_multiple_vkn(inputs);
    assert(results.size() == 3);
    assert(results[0].valid);
    assert(!results[1].valid);
    assert(!results[2].valid);
}

/* test_validate_multiple_vkn_empty boş VKN listesi davranışını test eder. */
void test_validate_multiple_vkn_empty() {
    const std::vector<std::string> inputs{};
    const auto results = tcknvkn::validate_multiple_vkn(inputs);
    assert(results.empty());
}

} // namespace

int main() {
    test_validate_tckn_valid();
    test_validate_tckn_normalize();
    test_validate_tckn_invalid_length();
    test_validate_tckn_leading_zero();
    test_validate_tckn_checksum();
    test_validate_tckn_same_digits();
    test_validate_tckn_multiple_errors();

    test_validate_vkn_valid();
    test_validate_vkn_normalize();
    test_validate_vkn_invalid_length();
    test_validate_vkn_checksum();
    test_validate_vkn_same_digits();

    test_validate_multiple_tckn();
    test_validate_multiple_tckn_empty();
    test_validate_multiple_vkn();
    test_validate_multiple_vkn_empty();

    std::cout << "All tests passed." << std::endl;
    return 0;
}
