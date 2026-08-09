# tcknvkn-cpp

C++ kütüphanesi ile TCKN (TC Kimlik No) ve VKN (Vergi Kimlik No) doğrulama işlemlerini sade bir API ile kullanabilirsiniz.

## Kurulum

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Hızlı Başlangıç

```cpp
#include "tcknvkn.hpp"

auto tckn = tcknvkn::validate_tckn("10000000146");
auto vkn = tcknvkn::validate_vkn("1000036109");
```

## API

- `ValidationResult validate_tckn(const std::string &input)`
- `std::vector<ValidationResult> validate_multiple_tckn(const std::vector<std::string> &inputs)`
- `ValidationResult validate_vkn(const std::string &input)`
- `std::vector<ValidationResult> validate_multiple_vkn(const std::vector<std::string> &inputs)`

## ValidationResult

```cpp
struct ValidationResult {
    bool valid;
    std::string value;
    std::vector<std::string> errors;
};
```

## Test

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

## Sık Kullanım İfadeleri

- tc üret
- tc uret
- tc no üret
- tc no uret
- tc oluştur
- tckn üret
- vkn üret
- vergi no üret
- vergi no oluşturucu
- vkn algoritması
- vkn doğrulama algoritması

## İlgili Bağlantılar

- Kütüphaneler: https://www.tcknvkn.com/kutuphaneler
- C++ kütüphane detayı: https://www.tcknvkn.com/kutuphaneler/cpp
- tc üret: https://www.tcknvkn.com/tc-uret
- tc uret: https://www.tcknvkn.com/tc-uret
- tc no üret: https://www.tcknvkn.com/tc-no-uret
- tc no uret: https://www.tcknvkn.com/tc-no-uret
- tc oluştur: https://www.tcknvkn.com/tc-uretici
- tckn üret: https://tcknvkn.com/tckn-uret
- vergi no üret: https://www.tcknvkn.com/vergi-no-uret
- vergi no oluşturucu: https://www.tcknvkn.com/vergi-no-uretici
- vkn üret: https://tcknvkn.com/vkn-uret
- vkn algoritması: https://www.tcknvkn.com/vergi-no-uret
- vkn doğrulama algoritması: https://www.tcknvkn.com/vergi-no-uretici

## Lisans

MIT
