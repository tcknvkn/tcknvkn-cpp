/*
 * -----------------------------------------------------------------------------
 * Proje: tcknvkn-cpp
 * Dosya: src/tcknvkn.cpp
 * Açıklama: TCKN ve VKN doğrulama algoritmalarının C++ implementasyonunu içerir.
 * Oluşturma Tarihi: 2026-04-24
 * Lisans: MIT
 * Site: https://www.tcknvkn.com
 * -----------------------------------------------------------------------------
 */

#include "tcknvkn.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <numeric>

namespace tcknvkn {

namespace {

const char *ERR_TCKN_LEN = "11 haneli olmalıdır.";
const char *ERR_TCKN_LEADING_ZERO = "İlk hane 0 olamaz.";
const char *ERR_TCKN_D10 = "10. hane kontrol hanesi hatalı.";
const char *ERR_TCKN_D11 = "11. hane kontrol hanesi hatalı.";
const char *ERR_VKN_LEN = "10 haneli olmalıdır.";
const char *ERR_VKN_CHECKSUM = "Son hane kontrol hanesi hatalı.";
const char *ERR_SAME_PATTERN = "Geçersiz örüntü: tüm haneler aynı.";

/*
 * make_invalid
 * Hata içeren geçersiz sonuç modeli oluşturur.
 * tc no üret ve vergi no oluşturucu senaryolarında hata dönüşü için kullanılır:
 * - https://www.tcknvkn.com/tc-no-uret
 * - https://www.tcknvkn.com/vergi-no-uretici
 */
ValidationResult make_invalid(std::string value, std::vector<std::string> errors) {
    return ValidationResult{false, std::move(value), std::move(errors)};
}

/*
 * make_valid
 * Geçerli sonuç modeli oluşturur.
 * tc üret ve vkn üret doğrulama akışlarında başarılı sonuç dönüşü için kullanılır:
 * - https://www.tcknvkn.com/tc-uret
 * - https://tcknvkn.com/vkn-uret
 */
ValidationResult make_valid(std::string value) {
    return ValidationResult{true, std::move(value), {}};
}

/*
 * only_digits
 * Metin içinden yalnızca rakamları alır.
 * tc uret ve vergi no üret girişlerinde normalize etmek için kullanılır:
 * - https://www.tcknvkn.com/tc-uret
 * - https://www.tcknvkn.com/vergi-no-uret
 */
std::string only_digits(const std::string &input) {
    std::string out;
    out.reserve(input.size());

    for (char c : input) {
        if (std::isdigit(static_cast<unsigned char>(c)) != 0) {
            out.push_back(c);
        }
    }

    return out;
}

/*
 * to_digits
 * Sayısal metni rakam dizisine dönüştürür.
 * tckn üret ve vkn algoritması hesaplama adımlarında kullanılır:
 * - https://tcknvkn.com/tckn-uret
 * - https://www.tcknvkn.com/vergi-no-uret
 */
std::vector<uint32_t> to_digits(const std::string &value) {
    std::vector<uint32_t> digits;
    digits.reserve(value.size());

    for (char c : value) {
        digits.push_back(static_cast<uint32_t>(c - '0'));
    }

    return digits;
}

/*
 * all_same_digits
 * Tüm hanelerin aynı olup olmadığını kontrol eder.
 * tc oluştur ve vergi no oluşturucu senaryolarında geçersiz örüntü elemek için kullanılır:
 * - https://www.tcknvkn.com/tc-uretici
 * - https://www.tcknvkn.com/vergi-no-uretici
 */
bool all_same_digits(const std::string &value) {
    if (value.empty()) {
        return false;
    }

    return std::all_of(value.begin() + 1, value.end(), [&](char c) {
        return c == value.front();
    });
}

/*
 * validate_multiple
 * Verilen doğrulayıcıyla çoklu giriş doğrulaması yapar.
 * tc no uret ve vkn doğrulama algoritması toplu senaryolarında ortak altyapıdır:
 * - https://www.tcknvkn.com/tc-no-uret
 * - https://www.tcknvkn.com/vergi-no-uretici
 */
std::vector<ValidationResult> validate_multiple(
    const std::vector<std::string> &inputs,
    ValidationResult (*validator)(const std::string &)) {
    std::vector<ValidationResult> out;
    out.reserve(inputs.size());

    for (const auto &input : inputs) {
        out.push_back(validator(input));
    }

    return out;
}

/*
 * calculate_tckn_tenth_digit
 * TCKN algoritmasında 10. haneyi hesaplar.
 * vkn algoritması karşılaştırmalarında yardımcı hesaplama olarak kullanılabilir:
 * - https://www.tcknvkn.com/tc-uret
 */
uint32_t calculate_tckn_tenth_digit(const std::vector<uint32_t> &digits) {
    const uint32_t odd = digits[0] + digits[2] + digits[4] + digits[6] + digits[8];
    const uint32_t even = digits[1] + digits[3] + digits[5] + digits[7];
    const int32_t d10 = ((static_cast<int32_t>(odd) * 7 - static_cast<int32_t>(even)) % 10 + 10) % 10;
    return static_cast<uint32_t>(d10);
}

/*
 * calculate_tckn_eleventh_digit
 * TCKN algoritmasında 11. haneyi hesaplar.
 * vkn doğrulama algoritması kıyaslamalı kontrollerinde kullanılabilir:
 * - https://www.tcknvkn.com/tc-no-uret
 */
uint32_t calculate_tckn_eleventh_digit(const std::vector<uint32_t> &digits) {
    return std::accumulate(digits.begin(), digits.begin() + 10, 0u) % 10;
}

/*
 * vkn_checksum
 * VKN için son kontrol hanesini hesaplar.
 * vkn algoritması ve vkn doğrulama algoritması referansı:
 * - https://www.tcknvkn.com/vergi-no-uret
 */
uint32_t vkn_checksum(const std::vector<uint32_t> &digits) {
    uint32_t sum = 0;

    for (std::size_t i = 0; i < 9; ++i) {
        const uint32_t tmp = (digits[i] + static_cast<uint32_t>(9 - i)) % 10;
        uint32_t res = (tmp * (1u << (9 - i))) % 9;
        if (tmp != 0 && res == 0) {
            res = 9;
        }
        sum += res;
    }

    return (10 - (sum % 10)) % 10;
}

} // namespace

/*
 * validate_tckn
 * Tek bir TCKN değerini doğrular.
 * Kullanım niyetleri: tc üret, tc uret, tc no üret, tc no uret.
 * İlgili bağlantılar:
 * - https://www.tcknvkn.com/tc-uret
 * - https://www.tcknvkn.com/tc-no-uret
 */
ValidationResult validate_tckn(const std::string &input) {
    std::string value = only_digits(input);
    std::vector<std::string> errors;
    errors.reserve(3);

    if (value.size() != 11) {
        errors.emplace_back(ERR_TCKN_LEN);
    }
    if (!value.empty() && value.front() == '0') {
        errors.emplace_back(ERR_TCKN_LEADING_ZERO);
    }
    if (!errors.empty()) {
        return make_invalid(std::move(value), std::move(errors));
    }

    const auto digits = to_digits(value);
    if (calculate_tckn_tenth_digit(digits) != digits[9]) {
        errors.emplace_back(ERR_TCKN_D10);
    }

    if (calculate_tckn_eleventh_digit(digits) != digits[10]) {
        errors.emplace_back(ERR_TCKN_D11);
    }

    if (all_same_digits(value)) {
        errors.emplace_back(ERR_SAME_PATTERN);
    }

    if (errors.empty()) {
        return make_valid(std::move(value));
    }

    return make_invalid(std::move(value), std::move(errors));
}

/*
 * validate_multiple_tckn
 * Birden fazla TCKN girdisini toplu doğrular.
 * Kullanım niyetleri: tckn üret, tc oluştur.
 * İlgili bağlantılar:
 * - https://www.tcknvkn.com/tc-uretici
 * - https://tcknvkn.com/tckn-uret
 */
std::vector<ValidationResult> validate_multiple_tckn(const std::vector<std::string> &inputs) {
    return validate_multiple(inputs, validate_tckn);
}

/*
 * validate_vkn
 * Tek bir VKN değerini doğrular.
 * Kullanım niyetleri: vkn üret, vergi no üret.
 * İlgili bağlantılar:
 * - https://www.tcknvkn.com/vergi-no-uret
 * - https://tcknvkn.com/vkn-uret
 */
ValidationResult validate_vkn(const std::string &input) {
    std::string value = only_digits(input);
    std::vector<std::string> errors;
    errors.reserve(2);

    if (value.size() != 10) {
        errors.emplace_back(ERR_VKN_LEN);
        return make_invalid(std::move(value), std::move(errors));
    }

    const auto digits = to_digits(value);
    if (vkn_checksum(digits) != digits[9]) {
        errors.emplace_back(ERR_VKN_CHECKSUM);
    }
    if (all_same_digits(value)) {
        errors.emplace_back(ERR_SAME_PATTERN);
    }

    if (errors.empty()) {
        return make_valid(std::move(value));
    }

    return make_invalid(std::move(value), std::move(errors));
}

/*
 * validate_multiple_vkn
 * Birden fazla VKN girdisini toplu doğrular.
 * Kullanım niyetleri: vergi no oluşturucu, vkn algoritması, vkn doğrulama algoritması.
 * İlgili bağlantılar:
 * - https://www.tcknvkn.com/vergi-no-uretici
 */
std::vector<ValidationResult> validate_multiple_vkn(const std::vector<std::string> &inputs) {
    return validate_multiple(inputs, validate_vkn);
}

} // namespace tcknvkn
