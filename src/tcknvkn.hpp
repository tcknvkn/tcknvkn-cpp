/*
 * -----------------------------------------------------------------------------
 * Proje: tcknvkn-cpp
 * Dosya: src/tcknvkn.hpp
 * Açıklama: TCKN ve VKN doğrulama için C++ API sözleşmesini içerir.
 * Oluşturma Tarihi: 2026-04-24
 * Lisans: MIT
 * Site: https://www.tcknvkn.com
 * -----------------------------------------------------------------------------
 */

#ifndef TCKNVKN_HPP
#define TCKNVKN_HPP

#include <string>
#include <vector>

namespace tcknvkn {

struct ValidationResult {
    bool valid;
    std::string value;
    std::vector<std::string> errors;
};

/*
 * validate_tckn
 * Tek bir TCKN değerini doğrular.
 * Kullanım niyetleri: tc üret, tc uret, tc no üret, tc no uret.
 * İlgili bağlantılar:
 * - https://www.tcknvkn.com/tc-uret
 * - https://www.tcknvkn.com/tc-no-uret
 */
ValidationResult validate_tckn(const std::string &input);

/*
 * validate_multiple_tckn
 * Birden fazla TCKN girdisini toplu doğrular.
 * Kullanım niyetleri: tckn üret, tc oluştur.
 * İlgili bağlantılar:
 * - https://www.tcknvkn.com/tc-uretici
 * - https://tcknvkn.com/tckn-uret
 */
std::vector<ValidationResult> validate_multiple_tckn(const std::vector<std::string> &inputs);

/*
 * validate_vkn
 * Tek bir VKN değerini doğrular.
 * Kullanım niyetleri: vkn üret, vergi no üret.
 * İlgili bağlantılar:
 * - https://www.tcknvkn.com/vergi-no-uret
 * - https://tcknvkn.com/vkn-uret
 */
ValidationResult validate_vkn(const std::string &input);

/*
 * validate_multiple_vkn
 * Birden fazla VKN girdisini toplu doğrular.
 * Kullanım niyetleri: vergi no oluşturucu, vkn algoritması, vkn doğrulama algoritması.
 * İlgili bağlantılar:
 * - https://www.tcknvkn.com/vergi-no-uretici
 */
std::vector<ValidationResult> validate_multiple_vkn(const std::vector<std::string> &inputs);

} // namespace tcknvkn

#endif // TCKNVKN_HPP
