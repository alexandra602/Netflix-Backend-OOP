# Netflix Backend Simulation 🎬

O aplicație C++ robustă care simulează logica de backend a unei platforme de video streaming, punând accent pe concepte avansate de Programare Orientată pe Obiecte (OOP) și Design Patterns.

## 📋 Despre Proiect
Acest proiect reprezintă o simulare complexă la nivel de consolă a unui sistem de tip Netflix. Scopul principal este implementarea corectă și eficientă a arhitecturilor software, a managementului memoriei și a principiilor SOLID prin intermediul C++ modern.

## ✨ Features

### 👤 Managementul Utilizatorilor
* **Sistem de Conturi:** Creare cont și autentificare securizată.
* **Sistem de Abonamente:** Gestionare planuri Standard și Premium.
* **Watchlist Personalizat:** Adăugare și urmărire filme/seriale dorite, specifice fiecărui utilizator.

### 📺 Interacțiune cu Conținutul (Filme și Seriale)
* **Ierarhii Complexe:** Suport pentru Filme, Seriale TV și Serii de Filme (rezolvând problema moștenirii în diamant).
* **Filtrare Avansată:** Căutare interactivă după gen sau rating minim dorit.
* **Sistem de Recomandări:** Generare aleatorie a conținutului sugerat și afișarea titlurilor cu rating maxim.
* **Personalizare la Runtime:** Modificarea calității video (HD/4K) sau adăugarea de subtitrări specifice direct din watchlist.

### 🎵 Playlist-uri și Notificări
* **Grupare Custom:** Utilizatorii își pot crea Playlist-uri personalizate pentru a rula conținutul preferat.
* **Alerte în Timp Real:** Abonare la seriale pentru a primi notificări la lansarea noilor episoade.

### 🛠️ Arhitectura si Concepte POO Aplicate
Proiectul demonstreaza stapanirea urmatoarelor concepte avansate:
* **Moștenire și Polimorfism:** Utilizarea claselor abstracte și a moștenirii virtuale pentru a evita ambiguitățile în ierarhie.
* **Smart Pointers:** Managementul automat al memoriei folosind shared_ptr și downcasting sigur.
* **Templates:** Crearea de clase generice pentru a scrie cod perfect reutilizabil.
* **Exception Handling:** Securizarea aplicației și validarea input-urilor folosind blocuri try-catch.

### 🧩 Design Patterns
* **Singleton:** Gestionează instanțele unice pentru tipurile de abonamente.
* **Observer:** Sistemul de notificări Push către utilizatori pentru lansări noi de episoade.
* **Decorator:** Extinde funcționalitatea claselor de bază pentru a adăuga opțiuni de Subtitrare și Calitate Video la runtime, fără a altera obiectul inițial.

## 👨‍💻 Autor
Panaet Maria Alexandra


**GitHub:** [@alexandra602]


**Proiect realizat pentru Facultatea de Matematica si Informatica, Universitatea din Bucuresti.**
