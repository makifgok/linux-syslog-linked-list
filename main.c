#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

/* * LogNode Yapýsý: Çift Yönlü Baðlý Listenin her bir düðümünü temsil eder.
 * Syslog içerisindeki her bir satýr metin olarak 'log_message' dizisinde tutulur.
 * 'prev' ve 'next' iþaretçileri ile çift yönlü gezinme saðlanýr.
 */
typedef struct LogNode {
    char log_message[MAX_LINE_LENGTH];
    struct LogNode* prev;
    struct LogNode* next;
} LogNode;

/* Baðlý listenin baþlangýç (head) ve son (tail) düðümlerini global olarak tanýmlýyoruz */
LogNode* head = NULL;
LogNode* tail = NULL;

/*
 * Fonksiyon: create_node
 * Amacý: Parametre olarak gelen log metnini içeren yeni bir düðüm (node) oluþturmak.
 * Dinamik bellek yönetimi (malloc) kullanýlarak bellek tahsisi yapýlýr.
 */
LogNode* create_node(const char* message) {
    LogNode* new_node = (LogNode*)malloc(sizeof(LogNode));
    if (new_node == NULL) {
        perror("Bellek tahsisi basarisiz oldu.");
        exit(EXIT_FAILURE);
    }
    // Gelen mesaji dugumun icindeki diziye kopyala
    strncpy(new_node->log_message, message, MAX_LINE_LENGTH);
    new_node->log_message[MAX_LINE_LENGTH - 1] = '\0'; // Guvenlik icin sonlandirici karakter
    new_node->prev = NULL;
    new_node->next = NULL;
    return new_node;
}

/*
 * Fonksiyon: append_log
 * Amacý: Oluþturulan yeni log düðümünü çift yönlü baðlý listenin sonuna (tail) eklemek.
 * Zaman Karmaþýklýðý: O(1) - Kuyruk iþaretçisi (tail) bilindiði için doðrudan ekleme yapýlýr.
 */
void append_log(const char* message) {
    LogNode* new_node = create_node(message);

    if (head == NULL) {
        // Liste bossa, head ve tail ayni dugumu isaret eder.
        head = new_node;
        tail = new_node;
    } else {
        // Liste doluysa, yeni dugumu tail'in sonuna ekle ve baglantilari guncelle.
        tail->next = new_node;
        new_node->prev = tail;
        tail = new_node;
    }
}

/*
 * Fonksiyon: print_logs_reverse
 * Amacý: Loglarý sondan baþa doðru okumak. (Çift yönlü baðlý liste seçme nedenimiz)
 * En güncel loglar listenin sonuna eklendiði için, analiz sýrasýnda en yeni kayýtlarý
 * ilk görmek amacýyla tail iþaretçisinden baþlayarak geriye (prev) doðru ilerler.
 */
void print_logs_reverse() {
    LogNode* current = tail;
    int count = 1;
    printf("\n--- SYSLOG KAYITLARI (GUNCELDEN ESKIYE) ---\n");
    while (current != NULL) {
        printf("[%d] %s", count, current->log_message);
        current = current->prev;
        count++;
    }
    printf("-------------------------------------------\n");
}

/*
 * Fonksiyon: free_list
 * Amacý: Program sonlanýrken heap üzerinde ayrýlmýþ olan düðüm belleklerini serbest býrakmak.
 * Memory Leak (bellek sýzýntýsý) oluþumunu engeller.
 */
void free_list() {
    LogNode* current = head;
    LogNode* temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
    tail = NULL;
}

int main() {
    /*
     * Gercek bir senaryoda bu dosya "/var/log/syslog" olacaktir. 
     * Ancak erisim izinleri (root) gerektirmemesi icin yerel bir test dosyasi okuyoruz.
     */
    FILE* file = fopen("syslog.txt", "r");
    if (file == NULL) {
        perror("Log dosyasi acilamadi! Lutfen ayni dizinde syslog.txt dosyasi olusturun.");
        return EXIT_FAILURE;
    }

    char buffer[MAX_LINE_LENGTH];

    // Dosyadan satir satir okuma islemi gerceklestiriliyor.
    while (fgets(buffer, MAX_LINE_LENGTH, file) != NULL) {
        // Okunan her bir log satiri cift yonlu bagli listeye eklenir.
        append_log(buffer);
    }

    fclose(file);

    // Bagli listeye alinan verilerin, en guncelden eskiye dogru terminale basilmasi.
    print_logs_reverse();

    // Islem bitiminde kullanilan dinamik belleklerin temizlenmesi.
    free_list();

    return EXIT_SUCCESS;
}
