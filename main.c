#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ODA_SAYISI 100

typedef struct{
    char *musteriAdi;
    char *musteriSoyadi;
}musteriBilgileri;

typedef struct{
    int gun;
    int ay;
    int yil;
}tarihBilgileri;

typedef struct{
    int kisiSayisi;
    int odaDurumu;
    int odaNumarasi;
    double fiyatBilgisi;
    musteriBilgileri musteri;
    tarihBilgileri girisTarihi;
    tarihBilgileri cikisTarihi;
}odaBilgileri;

typedef enum{
    musteri = 0, personel = 1
}yetkiSeviyeleri;

void yeniMusteriEkleme(odaBilgileri *);
int gunFarkiHesapla(tarihBilgileri, tarihBilgileri);
double fiyatHesaplama(odaBilgileri *, int);
void rezervasyonTemizle(odaBilgileri *, int);
void doluOdaBilgileriYazdirma(odaBilgileri *);
void odaBilgisiSorgulama(odaBilgileri *, int);
void fiyatBilgisiYazdirma();
void yedekKayitAlma(odaBilgileri *);

int main() {
    int belirlenenSifre=123456, kullanicininGirdigiSifre, odaNumarasi, i;
    yetkiSeviyeleri kullaniciYetkisi;
    char ch;

    odaBilgileri *odaDizisi;
    odaDizisi = (odaBilgileri *)calloc(ODA_SAYISI, sizeof(odaBilgileri));
    printf("\t\t\t\tOtel Rezervasyon Sistemine Hosgeldiniz...\n");
    printf("Lutfen burada calisiyorsaniz sifrenizi giriniz:");
    scanf("%d",&kullanicininGirdigiSifre);
    if(belirlenenSifre == kullanicininGirdigiSifre){
        kullaniciYetkisi = personel;
    }else{
        kullaniciYetkisi = musteri;
    }

    do{
        printf("1.Yeni musteri girisi yapmak istiyorsaniz 'E'\n");
        printf("2.Oda bilgisi sorgulamak istiyorsaniz 'S'\n");
        printf("3.Dolu oda bilgilerini goruntulemek istiyorsaniz 'G'\n");
        printf("4.Fiyat sorgusu yapmak istiyorsaniz 'F'\n");
        printf("5.Musteri cikisi gerceklestirmek istiyorsaniz 'C'\n");
        printf("6.Sistemden cikis yapmak istiyorsaniz 'Q' tuslayiniz\n");

        fflush(stdin);
        ch = getchar();
        switch(ch){
            case 'e':
            case 'E':
                if(kullaniciYetkisi == personel){
                    yeniMusteriEkleme(odaDizisi);
                }else{
                    printf("Bu alan sadece calisanlar tarafindan kullanilabilir\n");
                }
                break;
            case 's':
            case 'S':
                printf("Lutfen sorgulanacak olan odanin numarasini giriniz:");
                scanf("%d", &odaNumarasi);
                odaBilgisiSorgulama(odaDizisi, odaNumarasi);
                break;
            case 'g':
            case 'G':
                if(kullaniciYetkisi == personel){
                    doluOdaBilgileriYazdirma(odaDizisi);
                }else{
                    printf("Bu alan sadece calisanlar tarafindan kullanilabilir\n");
                }
                break;
            case 'f':
            case 'F':
                fiyatBilgisiYazdirma();
                break;
            case 'c':
            case 'C':
                if(kullaniciYetkisi == personel){
                    printf("Cikis yapmak istediginiz odanin numarasini giriniz\n");
                    scanf("%d",&odaNumarasi);
                    rezervasyonTemizle(odaDizisi,odaNumarasi);
                }else{
                    printf("Bu alan sadece calisanlar tarafindan kullanilabilir\n");
                }
                break;
            case 'q':
            case 'Q':
                yedekKayitAlma(odaDizisi);
                break;
            default:
                break;
        }
    }while(ch != 'Q');

    for(i=0; i < ODA_SAYISI; i++){
        if(odaDizisi[i].odaDurumu != 0){
            free(odaDizisi[i].musteri.musteriAdi);
            free(odaDizisi[i].musteri.musteriSoyadi);
        }
    }

    return 0;
}

void yeniMusteriEkleme(odaBilgileri *odalar){
    int sorgulanacakOda;
    char musteriAdi[20], musteriSoyadi[20],ch;
    printf("Kayit olmak istediginiz odayi giriniz:");
    scanf("%d", &sorgulanacakOda);
    odaBilgileri *oda = &odalar[sorgulanacakOda - 1];
    if(oda->odaDurumu != 0){
        printf("Maalesef. Kayit yaptirmak istediginiz oda %d:%d:%d tarihine kadar doludur.\n",oda->cikisTarihi.gun,oda->cikisTarihi.ay,oda->cikisTarihi.yil);
        return;
    }
    oda->odaNumarasi = sorgulanacakOda;

    printf("Ad ve soyad bilgilerinizi sirasiyla giriniz:");
    scanf("%s%s",&musteriAdi,&musteriSoyadi);
    oda->musteri.musteriAdi = (char *)malloc(sizeof(char) * strlen(musteriAdi) + 1);
    oda->musteri.musteriSoyadi = (char *)malloc(sizeof(char) * strlen(musteriSoyadi) + 1);
    strcpy(oda->musteri.musteriAdi,musteriAdi);
    strcpy(oda->musteri.musteriSoyadi,musteriSoyadi);
    printf("Kac kisi olacaginizi giriniz:");
    scanf("%d",&oda->kisiSayisi);
    printf("Giris tarihi bilgisini (gun:ay:yil) seklinde giriniz:");
    scanf("%d:%d:%d", &oda->girisTarihi.gun, &oda->girisTarihi.ay, &oda->girisTarihi.yil);
    printf("Cikis tarihi bilgisini (gun:ay:yil) seklinde giriniz:");
    scanf("%d:%d:%d", &oda->cikisTarihi.gun, &oda->cikisTarihi.ay, &oda->cikisTarihi.yil);
    oda->fiyatBilgisi = fiyatHesaplama(odalar, sorgulanacakOda);
    printf("Odemeniz gereken tutar:%.2lfTL\n", oda->fiyatBilgisi);
    printf("Onayliyor musunuz(E-H):");
    fflush(stdin);
    ch = getchar();
    if(ch == 'H'){
        rezervasyonTemizle(odalar,sorgulanacakOda);
        return;
    }
    oda->odaDurumu = 1;
}

int gunFarkiHesapla(tarihBilgileri girisTarihi,tarihBilgileri cikisTarihi){
    if(girisTarihi.gun > cikisTarihi.gun){
        cikisTarihi.ay -= 1;
        cikisTarihi.gun +=30;
    }
    if(girisTarihi.ay > cikisTarihi.ay){
        cikisTarihi.yil -= 1;
        cikisTarihi.ay += 12;
    }
    tarihBilgileri  tarihFarki;
    tarihFarki.gun = cikisTarihi.gun - girisTarihi.gun;
    tarihFarki.ay = cikisTarihi.ay - girisTarihi.ay;
    tarihFarki.yil = cikisTarihi.yil - girisTarihi.yil;

    int gunFarki = tarihFarki.yil * 365 + tarihFarki.ay * 30 + tarihFarki.gun;
    return gunFarki;
}

double fiyatHesaplama(odaBilgileri *odalar, int odaNumarasi){
    odaBilgileri *oda = &odalar[odaNumarasi - 1];
    int gunFarki = gunFarkiHesapla(oda->girisTarihi, oda->cikisTarihi);
    printf("Kalacaginiz gun sayisi:%d\n",gunFarki);

    return gunFarki * 150 * oda->kisiSayisi;
}

void rezervasyonTemizle(odaBilgileri * odalar,int odaNumarasi){
    odaBilgileri *oda = &odalar[odaNumarasi - 1];
    oda->odaDurumu = 0;
    free(oda->musteri.musteriAdi);
    free(oda->musteri.musteriSoyadi);
    printf("%d numarali odanin cikis/iptal islemi tamamlanmistir\n", odaNumarasi);

}

void doluOdaBilgileriYazdirma(odaBilgileri *odalar){
    int i;
    for(i = 0; i < ODA_SAYISI; i++){
        if(odalar[i].odaDurumu != 0){
            printf("-------------------------------\n");
            printf("Oda numarasi %d\n",odalar[i].odaNumarasi);
            printf("Musteri Adi-Soyadi:%s %s\n",odalar[i].musteri.musteriAdi, odalar[i].musteri.musteriSoyadi);
            printf("Kisi sayisi:%d\n",odalar[i].kisiSayisi);
            printf("Fiyat bilgisi:%.2lf\n",odalar[i].fiyatBilgisi);
            printf("Giris Tarihi:%d:%d:%d\n",odalar[i].girisTarihi.gun, odalar[i].girisTarihi.ay, odalar[i].girisTarihi.yil);
            printf("Cikis Tarihi:%d:%d:%d\n",odalar[i].cikisTarihi.gun, odalar[i].cikisTarihi.ay, odalar[i].cikisTarihi.yil);
        }
    }
}

void odaBilgisiSorgulama(odaBilgileri *odalar, int odaNumarasi){
    odaBilgileri *oda = &odalar[odaNumarasi - 1];
    if(oda->odaDurumu != 0){
        printf("%d numarali oda %d:%d:%d tarihine kadar doludur.\n",odaNumarasi, oda->cikisTarihi.gun, oda->cikisTarihi.ay, oda->cikisTarihi.yil);
    }else{
        printf("%d numarali oda su an bos durumdadir.\n", odaNumarasi);
    }
}

void fiyatBilgisiYazdirma(){
    int kisiSayisi;
    double toplamTutar;
    tarihBilgileri girisTarihi, cikisTarihi;

    printf("Kac kisi olacaginizi giriniz:");
    scanf("%d",&kisiSayisi);
    printf("Giris tarihi bilgisini (gun:ay:yil) seklinde giriniz:");
    scanf("%d:%d:%d", &girisTarihi.gun, &girisTarihi.ay, &girisTarihi.yil);
    printf("Cikis tarihi bilgisini (gun:ay:yil) seklinde giriniz:");
    scanf("%d:%d:%d", &cikisTarihi.gun, &cikisTarihi.ay, &cikisTarihi.yil);
    toplamTutar = gunFarkiHesapla(girisTarihi, cikisTarihi) * kisiSayisi * 150;
    printf("Odemeniz gereken toplam tutar: %.2lfTL", toplamTutar);
}

void yedekKayitAlma(odaBilgileri *odalar){
    int i;
    FILE *otelKayitlari = fopen("otelKayitlari.txt","a");
    if(otelKayitlari == NULL){
        fprintf(stderr, "Yedek kayit alinirken bir hata meydana geldi.\n");
        exit(1);
    }
    for(i=0; i<ODA_SAYISI; i++){
        if(odalar[i].odaDurumu != 0){
            fprintf(otelKayitlari,"--------------------------------------------------\n");
            fprintf(otelKayitlari, "Oda Numarasi:%d\n", odalar[i].odaNumarasi);
            fprintf(otelKayitlari,"Musterinin Adi-Soyadi:%s %s\n", odalar[i].musteri.musteriAdi,odalar[i].musteri.musteriSoyadi);
            fprintf(otelKayitlari,"Kisi sayisi:%d\n", odalar[i].kisiSayisi);
            fprintf(otelKayitlari, "Toplam tutar:%.2lfTL\n",odalar[i].fiyatBilgisi);
            fprintf(otelKayitlari, "Giris tarihi: %d:%d:%d\n", odalar[i].girisTarihi.gun, odalar[i].girisTarihi.ay, odalar[i].girisTarihi.yil);
            fprintf(otelKayitlari, "Cikis tarihi: %d:%d:%d\n", odalar[i].cikisTarihi.gun, odalar[i].cikisTarihi.ay, odalar[i].cikisTarihi.yil);

        }
    }
    fclose(otelKayitlari);
}


