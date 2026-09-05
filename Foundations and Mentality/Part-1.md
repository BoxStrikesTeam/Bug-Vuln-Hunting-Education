# Vulnerability Hunting & Bug Hunting: Temelleri

Uzun bir süredir hem açık kaynak hem de kapalı kaynak projelerde **Vulnerability Hunting** ve **Bug Hunting** yapıyorum. Bulduğum bulguları gerekli yerlere bildiriyor, bunlar üzerinden CVE'ler alıyor ve mevcut CVE kayıtlarına yeni bilgiler ekliyorum.

Bu süreç içerisinde fark ettiğim şeylerden biri, bu alanın zaman içerisinde benim için giderek daha anlaşılır ve sistematik hâle gelmesi oldu. Başlangıçta insanların gözünde oldukça karmaşık görünen bu alanın, doğru bir metodolojiyle yaklaşıldığında aslında öğrenilebilir ve sistematik bir süreç olduğunu düşünüyorum.

Bu nedenle bu forumda paylaşacağım yazılar ve hazırladığım videolar üzerinden sizlerle birlikte **Bug Hunting ve Vulnerability Hunting**, yani hata ve güvenlik açığı tespiti üzerine temel seviyeden başlayan bir eğitim serisi oluşturmak istiyorum.

Aynı konular üzerine videolar da hazırlıyorum. Yazıyı okuduktan sonra konuyu tekrar etmek isterseniz aşağıdaki YouTube videosuna da göz atabilirsiniz.

O hâlde en temelden başlayalım.

---

## Yazılım Bir Sözleşmedir

Bir yazılım projesini aslında bir tür **sözleşme** olarak düşünebiliriz.

Örneğin:

* Bu fonksiyon şu veriyi alır ve şu sonucu üretir.
* Bu dizi en fazla 100 eleman içerir.
* Bu bellek bloğu kullanıldıktan sonra serbest bırakılır.
* Bu fonksiyon hata durumunda belirli bir hata kodu döndürür.

Programın farklı parçaları arasında buna benzer çok sayıda varsayım ve sözleşme bulunur.

**Bug Hunting**, bu sözleşmelerin yerine getirilmediği noktaları bulma süreci olarak düşünülebilir.

Kod kendi belirlediği kuralları ihlal ettiğinde ortaya çıkan mantıksal veya davranışsal tutarsızlıklar birer bug olabilir.

Örneğin bir fonksiyon hata durumunda `-1` döndürüyor fakat çağıran kod bu dönüş değerini kontrol etmeden doğrudan bir dizi indeksi olarak kullanıyorsa burada bir bug vardır.

Başka bir örnek:

```c
for (int i = 0; i <= len; i++) {
    array[i] = 0;
}
```

Eğer dizinin uzunluğu `len` ise, son geçerli indeks `len - 1` olacaktır. `i <= len` kullanılması sonucunda sınırların dışına çıkılabilir. Bu da bir bug'dır.

Benzer şekilde:

```c
ptr = malloc(size);

if (...) {
    ...
}

memset(ptr, 0, size);
```

Eğer `malloc()` başarısız olur ve `NULL` dönerse, bu işaretçinin daha sonra kontrol edilmeden kullanılması **null pointer dereference** gibi bir hataya yol açabilir.

Buradaki temel soru şudur:

> **Kod kendi iddiasını yerine getirmiyor. Nerede ve neden yanlış yapıyor?**

Bug Hunting'in ilk aşamasında henüz güvenlik açısından düşünmek zorunda değiliz. Öncelikle **doğruluk, tutarlılık ve beklenen davranıştan sapma** arıyoruz.

---

## Vulnerability Hunting Nedir?

Şimdi ikinci aşamaya geçebiliriz.

Bir bug bulduk.

Peki bu hata kötü niyetli bir kullanıcı tarafından kontrollü bir şekilde tetiklenebiliyor mu?

Eğer bir hata saldırgan tarafından belirli girdilerle tetiklenebiliyor ve bunun sonucunda sistemin gizliliği, bütünlüğü veya erişilebilirliği üzerinde güvenlik açısından anlamlı bir etki ortaya çıkıyorsa, artık bu hatanın bir güvenlik açığına dönüşme ihtimalini incelemeye başlarız.

İşte **Vulnerability Hunting** burada devreye girer.

Vulnerability Hunting sırasında şu soruları sorarız:

* Saldırgan bu hatayı nasıl tetikleyebilir?
* Tetiklemek için hangi girdiyi veya koşulu kontrol edebilir?
* Hatanın sistem üzerindeki etkisi nedir?
* Sistem çökertilebilir mi?
* Yetki sınırları aşılabilir mi?
* Hassas bilgiler açığa çıkabilir mi?
* Hata başka bir zafiyetle birleştirilerek daha büyük bir saldırı zincirinin parçası olabilir mi?

Yani yalnızca:

> **Burada bir hata var mı?**

diye sormuyoruz.

Aynı zamanda:

> **Bu hata güvenlik açısından ne ifade ediyor?**

sorusunu soruyoruz.

---

## Bug ile Vulnerability Arasındaki İlişki

Bu alandaki en önemli ayrımlardan biri şudur:

> **Her bug güvenlik açığı değildir.**

Bir bug, programın beklenmeyen veya hatalı davranmasına neden olabilir ancak bunun güvenlik açısından kullanılabilir bir etkisi olmayabilir.

Örneğin bir programın belirli bir durumda yanlış hesaplama yapması bir bug olabilir. Ancak bu hatanın saldırgan tarafından kontrol edilebilir bir girdiden tetiklenmesi veya güvenlik sınırlarını aşması gerekmeyebilir.

Buna karşılık bir **buffer overflow**, **use-after-free**, **out-of-bounds access** veya uygun bağlamdaki bir **null pointer dereference**, belirli koşullar altında güvenlik açısından anlamlı sonuçlar doğurabilir.

Burada **attack surface** kavramı da önemlidir.

Attack surface, bir sistemin saldırgan tarafından etkileşime geçilebilen veya saldırı amacıyla kullanılabilecek giriş noktalarının bütününü ifade eder.

Bir bug'ın gerçekten güvenlik açığı olup olmadığını değerlendirirken, hatanın saldırı yüzeyiyle nasıl kesiştiğine bakarız.

Bu ayrımı yaparken temel olarak üç soru sorabiliriz.

### 1. Kontrol edilebilir mi?

Hatayı tetikleyen veriyi saldırgan kontrol edebiliyor mu?

Saldırgan bu koşulu kendi başına oluşturabiliyor mu?

### 2. Etkisi ne?

Hata başarılı bir şekilde tetiklendiğinde sistem üzerinde ne gibi bir etki oluşuyor?

* Çökme mi?
* Bilgi sızıntısı mı?
* Yetki ihlali mi?
* Veri bütünlüğünün bozulması mı?
* Başka bir güvenlik mekanizmasının aşılması mı?

### 3. Bir istismar zincirinin parçası olabilir mi?

Hata tek başına sınırlı bir etkiye sahip olsa bile başka bir bug veya güvenlik açığıyla birleştirildiğinde daha ciddi bir sonuca ulaşılabilir mi?

Bu sorular, basit bir bug ile gerçek bir güvenlik bulgusu arasındaki farkı anlamamıza yardımcı olur.

---

## Neden C ve C++?

Bu eğitim serisinde özellikle **C ve C++ projelerindeki hata ve güvenlik açıklarına** ağırlık vereceğiz.

Bunun önemli nedenlerinden biri bellek yönetiminin büyük ölçüde geliştiriciye bırakılmasıdır.

Belleğin yanlış yönetilmesi sonucunda:

* Buffer overflow
* Out-of-bounds access
* Use-after-free
* Double free
* Null pointer dereference
* Memory leak

gibi çeşitli hatalar ortaya çıkabilir.

Bu hataların tamamı doğrudan güvenlik açığı anlamına gelmez. Ancak bazıları, belirli koşullar altında saldırgan tarafından kontrol edilebilir hâle geldiğinde ciddi güvenlik sonuçlarına yol açabilir.

Bunun yanında C ve C++ projelerinde yalnızca bellek güvenliği problemlerine değil; mantıksal, algoritmik ve mimari hatalara da rastlayabiliriz.

Dolayısıyla amacımız yalnızca belirli bir bug sınıfını ezberlemek değil, **kodun davranışını analiz etmeyi öğrenmektir.**

---

## Özet

Kısaca özetlemek gerekirse:

**Bug Hunting** temel olarak:

> **Kod neden beklediğim gibi çalışmıyor?**

sorusuna cevap aramaktır.

**Vulnerability Hunting** ise bunun bir adım ötesine geçerek:

> **Bu hatalı davranış güvenlik açısından nasıl bir saldırı vektörüne dönüşebilir?**

sorusunu sormaktır.

İki kavram birbirinden farklıdır ancak birbirleriyle oldukça yakından ilişkilidir.

Bu ilk derste temel kavramları oturtmaya çalıştık. İlerleyen bölümlerde C ve C++ üzerinde gerçek kod örnekleri üzerinden ilerleyerek farklı bug sınıflarını, bunların nasıl tespit edilebileceğini ve hangi koşullarda güvenlik açığına dönüşebileceklerini inceleyeceğiz.

---

## Sonraki Bölümler

Bugünkü dersimiz bu kadar.

Konuyu tekrar etmek isteyenler aşağıdaki videodan da faydalanabilir.

[YouTube Videosu](https://www.youtube.com/watch?v=H-S6M5GBi00)

Okuduğunuz için teşekkürler.

**— Nullsans**
