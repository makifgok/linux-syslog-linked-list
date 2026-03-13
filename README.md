# Linux Syslog Bağlı Liste (Linked List) Uygulaması

Bu proje, Linux işletim sistemlerindeki sistem günlüklerini (`syslog`) C programlama dili kullanarak dinamik bir veri yapısı olan **Çift Yönlü Bağlı Liste (Doubly Linked List)** ile belleğe almayı ve işlemeyi amaçlayan akademik bir çalışmadır.

## 📝 Projenin Amacı ve Teknik Detaylar

### Syslog Nedir?
Syslog, Linux/Unix sistemlerde çalışan servislerin, çekirdeğin ve uygulamaların log (günlük) mesajlarını merkezi bir sistemde toplayan protokoldür. Sistem durumunun analizi ve hata ayıklama (debugging) süreçlerinde kritik rol oynar.

### Neden Çift Yönlü Bağlı Liste Kullanıldı?
Syslog dosyaları sürekli büyüyen ve çok sayıda satır barındıran yapılardır. Bu projede logların bellekte tutulması için **Çift Yönlü Bağlı Liste** tercih edilmiştir çünkü:
1. **Geriye Dönük Tarama:** Log analizinde genellikle en son oluşan (en güncel) hatalara bakılır. Çift yönlü bağlı listenin `prev` (önceki) işaretçisi sayesinde, listeye sondan (`tail`) başlanarak başa doğru O(1) maliyetle adım adım ilerlenebilir.
2. **Esnek Düğüm Yönetimi:** İlerleyen süreçlerde sadece belirli hata (ERROR) loglarını listede tutup diğerlerini silmek istediğimizde, önceki düğüme kolay erişim silme işlemlerini performanslı kılar.

## ⚙️ Kurulum ve Çalıştırma

Proje standart C kütüphanelerini kullanmaktadır. Herhangi bir harici kütüphane gerektirmez.

1. Repoyu bilgisayarınıza klonlayın:
   ```bash
   git clone [https://github.com/kullanici-adiniz/proje-repo-adi.git](https://github.com/kullanici-adiniz/proje-repo-adi.git)
