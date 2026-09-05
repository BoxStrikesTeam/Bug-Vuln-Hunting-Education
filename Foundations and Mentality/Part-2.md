# Vulnerability Hunting & Bug Hunting: Proje Seçimi, Bir Projeye Yaklaşım ve Mimarinin Önemi

Bir önceki yazıda **Bug Hunting** ve **Vulnerability Hunting** kavramlarının ne olduğunu ele almıştık.

Bu yazıda ise bir adım daha ileri giderek, bir güvenlik araştırmasına başlamadan önce cevaplanması gereken bazı önemli sorulara bakacağız.

Çünkü Vulnerability Hunting yalnızca bir projeyi açıp rastgele kod okumak değildir.

Öncelikle hangi projeye bakacağını seçmen gerekir. Daha sonra projeye nasıl yaklaşacağını belirlemen ve son olarak da sistemin genel yapısını anlaman gerekir.

Basitçe bu süreci üç parçaya ayırabiliriz:

* Doğru projeyi seçmek.
* Projeye sistematik bir şekilde yaklaşmak.
* Projenin mimarisini ve veri akışını anlamak.

Bu üçü birbirinden bağımsız değildir.

Önce neyi inceleyeceğini seçersin. Ardından seçtiğin projeye nereden başlayacağını belirlersin. Daha sonra ise kod parçalarının sistem içerisinde nasıl bir araya geldiğini anlamaya çalışırsın.

Bu nedenle bu üç konu, bir Vulnerability Hunter'ın araştırmaya başlamadan önce sahip olması gereken temel alışkanlıklardan bazılarıdır.

O hâlde başlayalım.

---

## 1.5. Proje Seçimi ve Hedef Belirleme Kriterleri

Açık kaynak dünyasında inceleyebileceğin milyonlarca proje bulunuyor.

Bu nedenle ilk soru oldukça basittir:

> **Hangi projeyi incelemeliyim?**

Burada amaç, karşına çıkan ilk projeyi seçmek değildir.

Bir projeye günlerini veya haftalarını harcamadan önce, o projenin araştırma açısından sana ne sunabileceğini değerlendirmek faydalı olacaktır.

### Proje aktif mi?

İlk bakılabilecek noktalardan biri projenin hâlâ aktif olarak geliştirilip geliştirilmediğidir.

Örneğin uzun süredir commit almayan, issue'ları cevaplanmayan veya geliştirme süreci neredeyse tamamen durmuş bir projede çalışmak bazı durumlarda verimsiz olabilir.

Çünkü bir güvenlik problemi bulduğunda, bunu bildirebileceğin ve düzeltebilecek aktif bir geliştirici topluluğunun bulunması önemlidir.

Projenin commit geçmişine basitçe bakabilirsin:

```bash
git log --since="6 months" --oneline | wc -l
```

Bu komut sana son altı ay içerisindeki commit sayısı hakkında kaba bir fikir verir.

Ancak burada belirli bir sayıya takılmamak gerekir.

Bir projenin aktif olup olmadığını yalnızca commit sayısıyla değerlendirmek doğru değildir. Bazı olgun projeler daha az değişiklik almasına rağmen aktif şekilde bakımı yapılıyor olabilir.

Buradaki temel soru şudur:

> **Bu proje hâlâ yaşıyor ve geliştiriliyor mu?**

Issue'lara verilen cevaplar, son sürümler, güvenlik güncellemeleri ve genel geliştirme aktivitesi bu konuda daha iyi bir fikir verebilir.

---

### Saldırı yüzeyi ne kadar açık?

İkinci önemli konu ise projenin dışarıdan ne kadar veri aldığıdır.

Bir Vulnerability Hunter için özellikle önemli olan noktalardan biri **untrusted data**, yani güvenilmeyen verinin sisteme nereden girdiğidir.

Örneğin bir dosya formatını işleyen parser düşünelim.

Program şöyle çalışıyor olabilir:

```text
Dosya
  ↓
Parser
  ↓
İç Veri Yapısı
  ↓
İşleme
```

Burada dosyanın içeriği dışarıdan gelmektedir.

Dolayısıyla parser, kullanıcı tarafından kontrol edilebilecek verilerle çalışıyordur.

JSON, XML, PNG, PDF ve benzeri formatları işleyen projeler bu nedenle araştırma açısından ilgi çekici olabilir.

Benzer şekilde network servislerinde de veri doğrudan dışarıdan gelir.

Örneğin:

```text
Network
   ↓
Socket
   ↓
Parser
   ↓
İşleme
```

SSH, HTTP veya DNS gibi protokollerle çalışan yazılımlarda dışarıdan gelen veri önemli bir saldırı yüzeyi oluşturabilir.

Komut satırı araçları da farklı bir saldırı yüzeyi sunabilir.

Örneğin bir program şu şekilde çalışıyorsa:

```bash
program input.file
```

buradaki `input.file` program tarafından işlenmektedir.

Dolayısıyla araştırmaya başlarken şu soruyu sorabiliriz:

> **Kullanıcı tarafından kontrol edilen veri bu sisteme nereden giriyor?**

Bu sorunun cevabı, çoğu zaman araştırmaya nereden başlaman gerektiği konusunda ilk yönü verir.

---

### Kullanılan programlama dili neden önemli?

Bir projenin kullandığı programlama dili de araştırma yaklaşımını etkileyebilir.

Özellikle C ve C++ projelerinde bellek yönetimi araştırma açısından önemli bir alan oluşturur.

C tarafında geliştirici doğrudan bellek yönetimiyle çalışabilir:

```c
malloc();
calloc();
realloc();
free();
```

Bu işlemlerin yanlış kullanılması sonucunda çeşitli hatalar ortaya çıkabilir.

Örneğin:

* Out-of-bounds access
* Use-after-free
* Double free
* Null pointer dereference
* Memory leak

gibi problemlerle karşılaşabiliriz.

Ancak burada önemli bir noktayı tekrar hatırlatmak gerekiyor:

> **Her bug bir vulnerability değildir.**

Bir bellek hatasının güvenlik açığı olarak değerlendirilebilmesi için, bulunduğu bağlamın ve etkisinin ayrıca incelenmesi gerekir.

C++ tarafında ise modern dil özellikleri bazı problemleri azaltırken farklı karmaşıklıklar ortaya çıkarabilir.

Örneğin bir nesnenin yaşam döngüsünü, ownership yapısını veya taşınma işlemlerini anlamak bazen doğrudan `malloc()` ve `free()` çağrılarını takip etmekten daha karmaşık olabilir.

Dolayısıyla burada amaç:

> **"C güvenlik açısından kötüdür, C++ güvenlidir."**

gibi bir sonuca ulaşmak değildir.

Asıl önemli olan, projenin kullandığı dilin ve mimarinin hangi tür hatalara daha yatkın olabileceğini anlamaktır.

---

### CVE geçmişine bakmak

Bir projeyi seçerken geçmiş güvenlik problemlerine bakmak da faydalı olabilir.

Örneğin bir projenin geçmişinde hiç CVE bulunmaması, o projenin mutlaka güvenli olduğu anlamına gelmez.

Belki proje gerçekten iyi tasarlanmıştır.

Belki de proje üzerinde yeterince güvenlik araştırması yapılmamıştır.

Bu nedenle:

> **CVE yok = Güvenli proje**

şeklinde bir çıkarım yapmak doğru değildir.

Buna karşılık daha önce güvenlik açıkları bulunan projelerde geçmiş yamaları incelemek oldukça öğretici olabilir.

Örneğin bir güvenlik açığı belirli bir parser içerisindeki uzunluk kontrolünden kaynaklanmış olabilir.

Geliştiriciler bu problemi bir noktada düzeltmiş olabilir.

Peki benzer mantık başka bir kod yolunda da kullanılıyor mu?

İşte burada **variant hunting**, yani düzeltilen bir hatanın benzerlerini başka noktalarda arama yaklaşımı önem kazanabilir.

Bu nedenle bir projenin geçmiş CVE kayıtları yalnızca "daha önce açık çıkmış" bilgisi olarak görülmemelidir.

Aynı zamanda projenin geçmişte hangi tür hatalara yatkın olduğunu anlamak için de kullanılabilir.

---

## 1.6. Bir Projeye Sıfırdan Yaklaşım

Bir proje seçtik ve kaynak kodunu indirdik.

Şimdi yapılabilecek en kolay hata, doğrudan rastgele dosyaları açıp kod okumaya başlamaktır.

Özellikle büyük projelerde binlerce dosya bulunabilir.

Bu nedenle araştırmaya sistematik bir şekilde yaklaşmak daha verimlidir.

### README ve dokümantasyonu oku

İlk olarak projenin ne yaptığını anlamaya çalış.

README ve dokümantasyon üzerinden şu soruların cevaplarını bulmaya çalışabilirsin:

* Program ne yapıyor?
* Hangi girdileri kabul ediyor?
* Hangi dosya formatlarını işliyor?
* Network üzerinden veri alıyor mu?
* Hangi protokolleri kullanıyor?
* Komut satırı argümanları neler?

Buradaki temel amaç yine güvenilmeyen verinin kaynağını bulmaktır.

Örneğin programın kullanımında şöyle bir komut görüyorsun:

```bash
program --file example.input
```

Buradan programın bir dosya işlediğini anlayabiliriz.

Bir başka projede:

```text
Client
   ↓
Server
```

yapısı bulunabilir.

Bu durumda network üzerinden gelen veriyle ilgilenmemiz gerekebilir.

Henüz kod okumadan bile projenin saldırı yüzeyi hakkında temel bir fikir oluşturmaya başlayabiliriz.

---

### Projeyi derle ve çalıştır

Daha sonra projeyi derlemeyi deneyebiliriz.

Örneğin:

```bash
./configure
make
```

veya:

```bash
mkdir build
cd build
cmake ..
make -j4
```

Projenin build sistemini çalıştırmak önemlidir.

Çünkü kaynak kodunu yalnızca okumak yerine, programı kendi ortamında çalıştırabilmek araştırmayı çok daha kolay hâle getirir.

Build çıktısını kaydetmek de faydalı olabilir:

```bash
make -j4 2>&1 | tee build.log
```

Derleyicinin verdiği warning'ler her zaman bir güvenlik açığı anlamına gelmez.

Ancak bazen hangi kod bölgelerine daha dikkatli bakılması gerektiği konusunda ipucu verebilir.

Örneğin:

```text
warning: implicit declaration of function ...
```

gibi bir uyarı gördüğümüzde bunun neden oluştuğunu inceleyebiliriz.

---

### Programı normal şekilde çalıştır

Programı derledikten sonra hemen karmaşık testlere geçmek zorunda değilsin.

Önce programın normal davranışını gözlemlemek faydalıdır.

Örneğin:

```bash
./program --help
```

veya:

```bash
./program sample.input
```

Program normal girdilerle nasıl çalışıyor?

Hangi çıktıları üretiyor?

Hata durumunda ne yapıyor?

Exit code'u ne?

Bu soruların cevaplarını gözlemlemeye başlayabilirsin.

Örneğin bir noktada:

```text
Segmentation fault
```

ile karşılaşırsan, elbette bunun neden oluştuğunu araştırmak isteyebilirsin.

Ancak tekrar belirtmek gerekir:

> **Bir crash görmek, tek başına yüksek etkili bir güvenlik açığı bulduğun anlamına gelmez.**

Crash yalnızca beklenmeyen bir davranış olduğunu gösterir.

Bunun güvenlik açısından ne anlama geldiğini ayrıca incelemek gerekir.

---

### Giriş vektörlerini haritala

Bir sonraki adımda programa verinin girebileceği noktaları çıkarmaya çalışabiliriz.

Örneğin:

* Komut satırı argümanları
* Ortam değişkenleri
* Standart giriş
* Dosyalar
* Network socket'leri

Kod içerisinde bunlarla ilişkili fonksiyonları arayabilirsin:

```text
argv
getopt
getenv
stdin
open
fopen
read
recv
```

Örneğin kendine şöyle basit bir not oluşturabilirsin:

```text
INPUT SOURCES

[COMMAND LINE]
argv
getopt

[ENVIRONMENT]
getenv

[FILES]
open
fopen
read

[NETWORK]
socket
recv

[STANDARD INPUT]
stdin
fgets
```

Bu liste, projenin saldırı yüzeyinin kaba bir haritasını oluşturabilir.

Artık rastgele kod okumak yerine şunu sorabiliriz:

> **Sisteme giren veri daha sonra nereye gidiyor?**

---

### Kritik kod bölgelerini bul

Burada amaç yine projenin tamamını baştan sona okumak değildir.

Bazı fonksiyonlar, özellikle C ve C++ projelerinde, araştırmaya başlanabilecek noktalar sağlayabilir.

Örneğin:

```text
strcpy
strcat
sprintf

malloc
calloc
realloc
free

system
popen
exec
```

Bu fonksiyonların kullanıldığı yerleri aramak mümkündür.

Örneğin:

```bash
grep -R "malloc" .
```

veya:

```bash
grep -R "strcpy" .
```

Ancak burada önemli bir hata yapmamak gerekiyor.

Bir dosyada `strcpy()` gördüğün anda:

> **"Açık buldum."**

diyemezsin.

Aynı şekilde:

```c
memcpy(dest, src, len);
```

satırı da tek başına bir güvenlik açığı değildir.

Şunları bilmemiz gerekir:

* `src` nereden geliyor?
* `dest` ne kadar büyük?
* `len` nereden geliyor?
* Uzunluk daha önce doğrulanmış mı?
* Bu fonksiyona hangi kod yolu üzerinden ulaşılıyor?

Ve bu bizi bir sonraki konuya götürüyor.

---

## 1.7. Proje Mimarisi ve Veri Akışını Anlamak

Bir fonksiyona tek başına bakmak çoğu zaman yeterli değildir.

Örneğin şöyle bir kodla karşılaştığını düşün:

```c
memcpy(dest, src, len);
```

Bu satır şüpheli görünebilir.

Ancak gerçekten bir güvenlik problemi olup olmadığını yalnızca bu satıra bakarak anlayamayız.

`len` sabit bir değer olabilir.

`src` daha önce doğrulanmış olabilir.

`dest` yeterince büyük olabilir.

Ya da tam tersine, `len` doğrudan network üzerinden gelen bir paketten alınmış olabilir.

Bu nedenle önemli olan yalnızca kod satırını görmek değil:

> **Bu kodun sistem içerisindeki yerini anlamaktır.**

---

### Çağrı akışını anlamak

Projeye bakarken `main()` fonksiyonundan başlayarak genel çağrı akışını çıkarmak faydalı olabilir.

Örneğin sistem kabaca şöyle çalışıyor olabilir:

```text
main()
  ↓
init()
  ↓
server_start()
  ↓
accept_connection()
  ↓
read_request()
  ↓
parse_request()
  ↓
handle_request()
```

Bu grafik sayesinde programın genel çalışma akışı hakkında fikir edinmeye başlarız.

Veri sisteme nereden giriyor?

Hangi fonksiyona gidiyor?

Nerede parse ediliyor?

Nerede işleniyor?

Bu soruların cevapları yavaş yavaş ortaya çıkar.

Büyük projelerde call graph çıkarmak için çeşitli araçlar kullanılabilir.

Ancak başlangıçta basitçe ana fonksiyonlardan başlayarak kendi çağrı haritanı çıkarmak bile oldukça faydalıdır.

---

### Kritik veri yapılarını bulmak

Bir sonraki adımda projenin önemli veri yapılarını anlamaya çalışabiliriz.

Örneğin bir HTTP sunucusunda:

```c
struct request
```

merkezi bir yapı olabilir.

Başka bir projede:

```c
struct packet
```

veya:

```c
struct frame
```

gibi yapılar sistem içerisinde sürekli dolaşıyor olabilir.

Burada şu sorular önemlidir:

* Bu yapı nerede oluşturuluyor?
* Hangi fonksiyonlara aktarılıyor?
* Pointer olarak mı kullanılıyor?
* Kopyalanıyor mu?
* Bellekten ne zaman kaldırılıyor?
* Ownership kimde?

Bu sorular özellikle nesne yaşam döngüsü ve bellek yönetimiyle ilgili hataları anlamak açısından önemlidir.

---

### Veri akışını takip etmek

Bence mimariyi anlamanın en önemli noktalarından biri veri akışını takip etmektir.

Örneğin network üzerinden gelen veri şöyle bir yol izliyor olabilir:

```text
recv()
  ↓
read_request()
  ↓
parse_header()
  ↓
parse_length()
  ↓
allocate_buffer()
  ↓
process_request()
```

Artık yalnızca son fonksiyona bakmıyoruz.

Verinin sisteme girdiği andan itibaren geçtiği yolu takip ediyoruz.

Burada şu soruları sorabiliriz:

> Bu veri nereden geliyor?

> Bu verinin uzunluğu nerede belirleniyor?

> Nerede doğrulanıyor?

> Nerede başka bir veri yapısına dönüştürülüyor?

> Nerede belleğe yazılıyor?

Bu sorular sayesinde çok daha anlamlı bir analiz yapabiliriz.

---

### Giriş → Parse → İşleme

Birçok projeyi anlamak için basit bir model kullanabiliriz:

```text
Giriş
  ↓
Parse
  ↓
İşleme
```

Örneğin:

```text
Network Packet
      ↓
parse_packet()
      ↓
struct request
      ↓
handle_request()
```

Başlangıçta dışarıdan gelen ve tamamen güvenilmeyen bir veri, birkaç fonksiyondan geçtikten sonra sistem içerisindeki sıradan bir değişkene dönüşebilir.

Örneğin:

```c
request->length
```

veya:

```c
buffer->size
```

gibi bir değer gördüğümüzde, bunun aslında başlangıçta nereden geldiğini anlamamız gerekir.

Çünkü bir değerin sistem içerisinde dolaşması, onun kaynağını değiştirmez.

Bir network paketinden gelen değer, birkaç fonksiyondan geçtikten sonra da aslında network paketinden gelmektedir.

Bu nedenle veri akışını takip etmek oldukça önemlidir.

---

## Sonuç

Bu yazıda aslında birbirine bağlı üç farklı konuyu ele aldık.

İlk olarak doğru projeyi seçmeye çalıştık.

Bir projenin aktifliği, saldırı yüzeyi, kullandığı teknolojiler ve geçmiş güvenlik problemleri bize araştırmaya başlamadan önce bazı ipuçları verebilir.

Daha sonra projeye sıfırdan nasıl yaklaşabileceğimizi ele aldık.

Dokümantasyonu okumak, projeyi derlemek, normal davranışını gözlemlemek ve giriş vektörlerini çıkarmak, doğrudan rastgele kod okumaktan daha sistematik bir başlangıç sağlayabilir.

Son olarak ise projenin mimarisini ve veri akışını anlamanın önemine baktık.

Çünkü tek başına:

```c
memcpy()
```

veya:

```c
malloc()
```

görmek bize henüz bir güvenlik açığı söylemez.

Asıl önemli soru şudur:

> **Bu veri buraya nereden geliyor ve sistem içerisinde buraya ulaşana kadar neler oluyor?**

Bir süre sonra kod incelerken kendine otomatik olarak şu soruları sormaya başladığını fark edebilirsin:

> Bu veri nereden geliyor?

> Bu uzunluk değerini kim kontrol ediyor?

> Bu pointer'ın sahibi kim?

> Bu bellek ne zaman serbest bırakılıyor?

> Bu veri başka bir fonksiyonda tekrar kullanılıyor mu?

> Başka bir kod yolu bu değeri değiştirebilir mi?

Vulnerability Hunting'in önemli bir kısmı, aslında bu soruları doğru yerde ve doğru zamanda sorabilmektir.

Amaç yalnızca şüpheli görünen bir kod satırı bulmak değildir.

Amaç, **kodun davranışını, mimarisini ve verinin sistem içerisindeki yolculuğunu anlayarak gerçekten anlamlı bir güvenlik problemi tespit edebilmektir.**

Bir sonraki bölümde ise biraz daha farklı bir noktaya geçeceğiz:

## 2.1. Yazılımcının Düşünme Hataları

Çünkü bazı güvenlik problemleri yalnızca bir satırın yanlış yazılmasından kaynaklanmaz.

Bazen asıl problem, yazılım geliştirilirken yapılan bir varsayımda veya yazılımcının sistemi düşünme biçiminde ortaya çıkar.

Okuduğunuz için teşekkürler.
