#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <random>
#include <stdexcept>
#include <limits>
#include <ctime>

// ierarhia 1: Content -> Movie | TV Series -> Movie Series (mostenire in diamant)
class Content {
protected:
    std::string title;
    std::string genre;
    int year; // anul in care a fost lansat
    double rating; // rating de la 0 la 10, 0 = niciun rating, 10 = rating maximal
public:
    Content(const std::string& title = "", const std::string& genre = "", int year = 0, double rating = 0) : title(title), genre(genre), year(year), rating(rating) {} // constructor
    Content(const Content& other) : title(other.title), genre(other.genre), year(other.year), rating(other.rating) {} // constructor de copiere
    Content& operator=(const Content& other) { // operator de atribuire
        if (this != &other) {
            title = other.title;
            genre = other.genre;
            year = other.year;
            rating = other.rating;
        }
        return *this;
    }
    virtual ~Content() = default; // destructor

    virtual void ShowDetails() const = 0; // metoda virtuala pe care o voi implementa in clasele derivate -> clasa abstracta

    friend std::ostream& operator<<(std::ostream& os, const Content& c) { // afisare
        os << "Titlu: " << c.title << std::endl;
        os << "Gen: " << c.genre << std::endl;
        os << "An: " << c.year << " | ";
        os << "Rating: " << c.rating << std::endl;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Content& c) { // citire
        std::cout << "Titlu: ";
        is >> c.title;
        std::cout << "Gen: ";
        is >> c.genre;
        std::cout << "An: ";
        is >> c.year;
        std::cout << "Rating: ";
        is >> c.rating;
    }

    bool operator<(const Content& other) const { // compar rating-urile continuturilor pentru a obtine titlul de rating maxim
        return this->rating < other.rating;
    }

    bool operator==(const Content& other) const { // compar titlurile pentru a verifica daca un continut apare in vectorul de continut disponibil
        return this->title == other.title;
    }

    // setteri
    void SetRating(double rating) { this->rating = rating;}
    void SetTitle(const std::string& title) { this->title = title;}
    void SetGenre(const std::string& genre) { this->genre = genre;}
    void SetYear(int year) { this->year = year;}

    // getteri
    [[nodiscard]] std::string GetTitle() const { return title;}
    [[nodiscard]] std::string GetGenre() const { return genre;}
    [[nodiscard]] int GetYear() const { return year;}
    [[nodiscard]] double GetRating() const { return rating;}
};

class Movie : virtual public Content {
protected:
    std::string director;
    int duration; // durata exprimata in minute
public:
    Movie(const std::string& title = "", const std::string& genre = "", int year = 0, double rating = 0, const std::string& director = "", int duration = 0) : Content(title, genre, year, rating), director(director), duration(duration) {} // constructor
    Movie(const Movie& other) : Content(other), director(other.director), duration(other.duration) {} // constructor de copiere
    Movie& operator=(const Movie& other) { // operator de atribuire
        if (this != &other) {
            Content::operator=(other);
            director = other.director;
            duration = other.duration;
        }
        return *this;
    }
    ~Movie() override = default; // destructor

    void ShowDetails() const override { // afisez detaliile filmului
        std::cout << "[FILM]: " << *this << std::endl;
        //std::cout << "Regizor: " << director << std::endl;
        //std::cout << "Durata: " << duration << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const Movie& m) { // afisare
        os << "Titlu: " << m.title << std::endl;
        os << "Gen: " << m.genre << std::endl;
        os << "An: " << m.year << " | ";
        os << "Rating: " << m.rating << std::endl;
        os << "Regizor: " << m.director << std::endl;
        os << "Durata: " << m.duration << std::endl;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Movie& m) { // citire
        is >> static_cast<Content&>(m); // il convertesc pe m la tipul content pentru a putea folosi citirea din clasa de baza
        std::cout << "Regizor: ";
        is >> m.director;
        std::cout << "Durata: ";
        is >> m.duration;
        return is;
    }

    // setteri
    void SetDirector(const std::string& director) { this->director = director;} // setteri pentru regizor
    void SetDuration(int duration) { this->duration = duration;} // setter pentru durata

    // getteri
    [[nodiscard]] std::string GetDirector() const { return director;} // getteri pentru regizor
    [[nodiscard]] int GetDuration() const { return duration;} // getter penrtu durata
};

class Observer { // Observer Pattern
public:
    virtual void update(const std::string& message) = 0;
    virtual ~Observer() = default;
};

class Observable {
protected:
    std::vector<std::shared_ptr<Observer>> observers; // observerii care trebuiesc notificati
public:
    void AddObserver(std::shared_ptr<Observer> o) { // adaug un nou observer
        observers.push_back(o);
    }
    void Notify(const std::string& m) { // dau notificari tuturor observerilor
        for (auto& o : observers) {
            o->update(m);
        }
    }
};

class TVseries : virtual public Content, public Observable {
protected:
    int nr_of_episodes;
public:
    TVseries(const std::string& title = "", const std::string& genre = "", int year = 0, double rating = 0, int nr_of_episodes = 0) : Content(title, genre, year, rating), nr_of_episodes(nr_of_episodes) {} // constructor
    TVseries(const TVseries& other) : Content(other), nr_of_episodes(other.nr_of_episodes) {} // constructor de copiere
    TVseries& operator=(const TVseries& other) { // operator de atribuire
        if (this != &other) {
            Content::operator=(other);
            nr_of_episodes = other.nr_of_episodes;
        }
        return *this;
    }
    ~TVseries() override = default; // destructor

    void ShowDetails() const override { // afisez detaliile seriei TV
        std::cout << "[SERIE TV]: " << title << std::endl;
        std::cout << "Gen: " << genre << std::endl;
        std::cout << "Numarul de episoade: " << nr_of_episodes << std::endl;
        std::cout << "An: " << year << " | Rating: " << rating << std::endl;
    }

    void ReleaseNewEpisode() { // pentru Observer: apare un nou episod intr-un serial -> trimit notificari tuturor userilor abonati la acel serial
        Notify("Episod nou in " + title);
    }
    friend std::ostream& operator<<(std::ostream& os, const TVseries& t) { // afisare
        os << "Ttilu: " << t.title << std::endl;
        os << "Gen: " << t.genre << std::endl;
        os << "An: " << t.year << " | ";
        os << "Rating: " << t.rating << std::endl;
        os << "Numarul de episoade: " << t.nr_of_episodes << std::endl;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, TVseries& t) { // citire
        is >> static_cast<Content&>(t);
        std::cout << "Numarul de episoade: ";
        is >> t.nr_of_episodes;
        return is;
    }

    void SetNrOfEpisodes(int nr_of_episodes) { this->nr_of_episodes = nr_of_episodes;} // setteri pentru numarul de episoade
    [[nodiscard]] int GetNrOfEpisodes() const { return nr_of_episodes;} // getteri pentru numarul de episoade
};

class MovieSeries : public Movie, public TVseries {
//durata va reprezenta durata medie a filmelor
//nr de episoade va reprezenta nr de filme din serie
//anul va reprezenta anul aparitiei primului film din serie
public:
    MovieSeries(const std::string& title = "", const std::string& genre = "", int duration = 0, int year = 0, double rating = 0, const std::string& director = "", int nr_of_episodes = 0) : Content(title, genre, year, rating), Movie(title, genre, year, rating, director, duration), TVseries(title, genre, year, rating, nr_of_episodes) {} // constructor
    MovieSeries(const MovieSeries& other) : Content(other), Movie(other), TVseries(other) {} // constructor de copiere
    MovieSeries& operator=(const MovieSeries& other) { // operator de atribuire
        if (this != &other) {
            Movie::operator=(other);
            TVseries::operator=(other);
        }
        return *this;
    }
    ~MovieSeries() override = default; // destructor

    friend std::ostream& operator<<(std::ostream& os, const MovieSeries& ms) { // afisare
        os << "Ttilu: " << ms.title << std::endl;
        os << "Gen: " << ms.genre << std::endl;
        os << "Durata: " << ms.duration << std::endl;
        os << "An: " << ms.year << " | ";
        os << "Rating: " << ms.rating << std::endl;
        os << "Regizor: " << ms.director << std::endl;
        os << "Numarul de episoade: " << ms.nr_of_episodes << std::endl;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, MovieSeries& ms) { // citire
        is >> static_cast<Content&>(ms);
        std::cout << "Regizor: ";
        is >> ms.director;
        std::cout << "Durata: ";
        is >> ms.duration;
        std::cout << "Numarul de episoade: ";
        is >> ms.nr_of_episodes;
        return is;
    }

    void ShowDetails() const override { // afisez detaliile seriei de filme
        std::cout << "[SERIE DE FILME]: " << title << std::endl;
        std::cout << "Regizor: " << director << std::endl;
        std::cout << "Gen: " << genre << std::endl;
        std::cout << "Numarul de filme: " << nr_of_episodes << std::endl;
        std::cout << "An aparitie: " << year << " | Rating: " << rating << std::endl;
    }
};

// ierarhia 2: Subscription -> Premium Subscription

class Subscription {
protected:
    std::string type;
    double price;
public:
    Subscription(const std::string& type = "Standard", double price = 29.99) : type(type), price(price) {} // constructor
    Subscription(const Subscription& other) : type(other.type), price(other.price) {} // constructor de copiere
    Subscription& operator=(const Subscription& other) { // operator de atribuire
        if (this != &other) {
            type = other.type;
            price = other.price;
        }
        return *this;
    }
    virtual ~Subscription() = default; // destructor

    virtual void ShowDetails() const { // afisez detalii despre abonament
        std::cout << "Tipul abonamentului: " << type;
        std::cout << " | Pretul abonamentului: " << price << " lei" << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const Subscription& s) { // afisare
        os << "Tipul abonamentului: " << s.type << " | ";
        os << "Pretul abonamentului: " << s.price << " lei" << std::endl;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Subscription& s) { // citire
        std::cout << "Tipul abonamentului: ";
        is >> s.type;
        std::cout << "Pretul abonamentului: ";
        is >> s.price;
        return is;
    }

    // setteri
    void SetType(const std::string& type) { this->type = type;}
    void SetPrice(double price) { this->price = price;}

    // getteri
    [[nodiscard]] std::string GetType() const { return type;}
    [[nodiscard]] double GetPrice() const { return price;}
};

class Premium_Subscription : public Subscription {
public:
    Premium_Subscription(const std::string& type = "Premium", double price = 49.99) : Subscription(type, price) {} // constructor
    Premium_Subscription(const Premium_Subscription& other) : Subscription(other) {} // constructor de copiere
    Premium_Subscription& operator=(const Premium_Subscription& other) { // operator de atribuire
        if (this != &other) {
            Subscription::operator=(other);
        }
        return *this;
    }

    void ShowDetails() const override { // afisez detalii despre abonament premium
        Subscription::ShowDetails();
    }
};

// ierarhia 3: Person -> User

class Person {
protected:
    std::string name;
public:
    Person(const std::string& name = "") : name(name) {} // constructor
    Person(const Person& other) : name(other.name) {} // constructor de copiere
    Person& operator=(const Person& other) { // operator de atribuire
        if (this != &other) {
            name = other.name;
        }
        return *this;
    }
    virtual ~Person() = default; // destructor

    virtual void ShowDetails() const { // afisez detalii despre persoana
        std::cout << "Nume: " << name << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const Person& p) { // afisare
        os << "Nume: " << p.name << std::endl;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, Person& p) { // citire
        std::cout << "Nume: ";
        is >> p.name;
        return is;
    }

    // setteri
    void SetName(const std::string& name) { this->name = name;}

    // getteri
    [[nodiscard]] std::string GetName() const { return name;}
};

// userii isi pot crea playlist uri personalizate (un fel de spotify, dar cu filme si seriale)
template <typename T>
class Playlist {
    std::string name; // numele listei de redare
    std::vector<std::shared_ptr<T>> content_list;
public:
    Playlist(const std::string& n) : name(n) {} // constructor
    void AddItem(std::shared_ptr<T> item) { // adaug un titlu nou in lista de redare
        content_list.push_back(item);
    }
    void play() const { // userul da play la lista de redare(filmele se redau un ordine)
        std::cout << "Playlist-ul " << name << " este redat" << std::endl;
        for (auto& c : content_list) {
            c->ShowDetails();
        }
    }
    void display() const { // afisez continutul din playlist
        std::cout << "Playlist " << name << ": " << std::endl;
        for (auto& c : content_list) {
            c->ShowDetails();
        }
        std::cout << std::endl;
    }
};

// utilizatorii sunt derivati din persoane, dar fiecare user are o parola, un watchlist si playlist-uri personalizate
class User : public Person {
protected:
    std::string email;
    std::string password;
    std::shared_ptr<Subscription> subscript;
    std::vector<std::shared_ptr<Content>> watchlist;
    std::vector<Playlist<Content>> playlist;
public:
    User(const std::string& name = "", const std::string& email = "",const std::string& password = "", std::shared_ptr<Subscription> subscript = std::make_shared<Subscription>()) : Person(name), email(email), password(password), subscript(subscript) {} // constructor
    User(const User& other) : Person(other), email(other.email), password(other.password), subscript(other.subscript) {} // constructor de copiere
    User& operator=(const User& other) { // operator de atribuire
        if (this != &other) {
            Person::operator=(other);
            email = other.email;
            password = other.password;
            subscript = other.subscript ? std::make_shared<Subscription>(*other.subscript) : nullptr;
        }
        return *this;
    }
    virtual ~User() = default; // destructor

    void ShowDetails() const override { // afisez detalii despre utilizator(mai putin parola)
        std::cout << "Nume: " << name << std::endl;
        std::cout << "Email: " << email << std::endl;
        if (subscript) {
            std::cout << "Abonament: ";
            subscript->ShowDetails();
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const User& u) { // afisare
        os << "Nume: " << u.name << std::endl;
        os << "Email: " << u.email << std::endl;
        os << "Password: " << u.password << std::endl;
        if (u.subscript) {
            os << "Abonament: ";
            os << *u.subscript;
        }
        return os;
    }
    friend std::istream& operator>>(std::istream& is, User& u) { // citire
        std::cout << "Nume: ";
        is >> u.name;
        std::cout << "Email: ";
        is >> u.email;
        std::cout << "Parola: ";
        is >> u.password;
        // abonamentul va fixsetat ulterior
        return is;
    }

    // setteri
    void SetEmail(const std::string& email) { this->email = email;}
    void SetPassword(const std::string& password) { this->password = password;}
    void SetSubscript(std::shared_ptr<Subscription> subscript) { this->subscript = subscript;}

    // getteri
    [[nodiscard]] std::string GetEmail() const { return email;}
    [[nodiscard]] std::string GetPassword() const { return password;}
    [[nodiscard]] std::shared_ptr<Subscription> GetSubscript() const { return subscript;}

    // functii utile pentru meniu
    bool login(const std::string& e, const std::string& p) { // utilizatorul se logheaza
        return e == email && p == password;
    }
    void add_to_watchlist(std::shared_ptr<Content> c) { // adaug continut in watchlist
        watchlist.push_back(c);
    }
    void show_watchlist() const { // afisez tot continutul watchlist ului
        if (watchlist.empty())std::cout << "Watchlist-ul este gol.\n";
        else {
               std::cout << "Watchlist-ul tau:\n";
               int i=0;
               for (auto& w : watchlist) {
                   // w->ShowDetails(); - varianta fara downcasting
                   // downcasting la clasa movie
                   std::cout << i << ". ";
                   i++;
                   auto decorated = std::dynamic_pointer_cast<Content>(w); // verific daca continutul a fost decorat (daca au fost adaugate subtitrari sau a fost schimbata calitatea)
                   if (decorated) {
                       decorated->ShowDetails(); // daca da, atunci folosesc afisarea specifica continutului decorat
                   }
                   else { // altfel, folosesc afisarea standard
                       std::shared_ptr<Movie> m = std::dynamic_pointer_cast<Movie>(w);
                       if (m) {
                           std::cout << "- [FILM] " << m->GetTitle() << "\n" << "Regizor: " << m->GetDirector() << " | Durata: " << m->GetDuration() << " minute" << " | Gen: " << m->GetGenre() << "\n"; ;
                           continue;
                       }
                       // downcasting la clasa TVseries
                       std::shared_ptr<TVseries> s = std::dynamic_pointer_cast<TVseries>(w);
                       if (s) {
                           std::cout << "- [SERIAL] " << s->GetTitle() << "\n" << "Nr. de episoade: " << s->GetNrOfEpisodes() << " | Gen: " << s->GetGenre() << "\n"; ;
                           continue;
                       }
                       // downcasting la clasa MovieSeries
                       std::shared_ptr<MovieSeries> ms = std::dynamic_pointer_cast<MovieSeries>(w);
                       if (ms) {
                           std::cout << "- [SERIE DE FILME] " << ms->GetTitle() << "\n" << "Regizor: " << ms->GetDirector() << " | Nr. de episoade: " << ms->GetNrOfEpisodes() << " | Gen: " << ms->GetGenre() << "\n"; ;
                           continue;
                       }
                       // daca nu a reusit niciun cast
                       std::cout << "- [CONTINUT NECUNOSCUT] " << w->GetTitle() << "\n";
                   }
               }
        }
    }
    void show_subcription() const{ // functie pentru afisarea detaliilor despre abonamentul utilizatorului
        subscript->ShowDetails();
    }
    void update_watchlist(int index, std::shared_ptr<Content> new_content) { // actualizez continuturile din watchlist (daca au fost decorate)
        if (index >=0 && index < watchlist.size()) {
            watchlist[index] = new_content;
        }
    }
    [[nodiscard]] const std::vector<std::shared_ptr<Content>>& GetWatchlist() const { return watchlist; } // returnez watchlist ul
    [[nodiscard]] const std::vector<Playlist<Content>>& GetPlaylist() const { return playlist; } // returnez playlist urile
    void CreatePlaylist(const std::string& name) { // creez un nou playlist (numele il citesc de la tastatura, in meniu)
        playlist.emplace_back(name);
    }
    void AddToPlaylist(int playlist_index, std::shared_ptr<Content> item) { // adaug continut intr-un playlist
        if (playlist_index >= 0 && playlist_index < playlist.size()) {
            playlist[playlist_index].AddItem(item);
        }
    }
    void ShowPlaylists() const { // afisez listele de redare are utilizatorului curent
        for (size_t i = 0; i < playlist.size(); i++) {
            std::cout << i << ". ";
            playlist[i].display();
            std::cout << std::endl;
        }
    }
    void PlayPlaylist(int playlist_index) { // utilizatorul reda un anumit playlist
        if (playlist_index >= 0 && playlist_index < playlist.size()) {
            playlist[playlist_index].play();
        }
        else {
            throw std::invalid_argument("Index invalif!");
        }
    }
};

class UserObserver : public Observer { // clasa pentru a notifica toti utilizatorii abonati la un serial ca a aparut un nou episod
    User* user_to_notify;
public:
    UserObserver(User* u) : user_to_notify(u) {}
    void update(const std::string &message) override {
        if (user_to_notify) {
            std::cout << "[NOTIFICARE pentru " << user_to_notify->GetName() << "] " << message << std::endl;
        }
    }
};

// filtrez continuturile
template <typename T>
class Filter {
    std::vector<std::shared_ptr<T>> content_list;
public:
    Filter(std::vector<std::shared_ptr<T>>& contents) : content_list(contents) {} // constructor
    // Filtrare dupa gen
    std::vector<std::shared_ptr<T>> ByGenre(const std::string& genre) {
        std::vector<std::shared_ptr<T>> result; // pun toate continuturile care se potrivesc cu ce cauta utilizatorul intr-un vector
        for (auto& c : content_list) {
            if (c->GetGenre() == genre) {
                result.push_back(c);
            }
        }
        return result;
    }
    // Filtrare dupa rating
    std::vector<std::shared_ptr<T>> ByRating(double min_rating) {
        std::vector<std::shared_ptr<T>> result; // pun toate continuturile care se potrivesc cu ce cauta utilizatorul intr-un vector
        for (auto& c : content_list) {
            if (c->GetRating() >= min_rating) {
                result.push_back(c);
            }
        }
        return result;
    }
};

class SubcriptionManager { // Singleton pentru abonamente
    static std::shared_ptr<Subscription> basic_subscription;
    static std::shared_ptr<Subscription> premium_subcription;
    SubcriptionManager() = default; // constructor privat pentru a ma asigura ca se creeaza o singura instanta a clasei
public:
    static std::shared_ptr<Subscription> GetBasicSubscription() { // utilizatorul vrea abonament standard
        if (!basic_subscription) {
            basic_subscription = std::make_shared<Subscription>("Standard", 29.99);
        }
        return basic_subscription;
    }
    static std::shared_ptr<Subscription> GetPremiumSubscription() { // utilizatorul vrea abonament premium
        if (!premium_subcription) {
            premium_subcription = std::make_shared<Premium_Subscription>("Premium", 49.99);
        }
        return premium_subcription;
    }
};

// initializez membrii statici ai clasei
std::shared_ptr<Subscription> SubcriptionManager::basic_subscription = nullptr;
std::shared_ptr<Subscription> SubcriptionManager::premium_subcription = nullptr;

class ContentDecorator : public Content { // decorez continutul
protected:
    std::shared_ptr<Content> decorated_content;
public:
    ContentDecorator(std::shared_ptr<Content> content) : decorated_content(content) {} // constructor
    void ShowDetails() const override { // afisez continutul decorat
        decorated_content->ShowDetails();
    }
};

class SubtitleDecorator : public ContentDecorator { // adaug subtitrari continutului din watchlist
    std::string language;
public:
    SubtitleDecorator(std::shared_ptr<Content> c, const std::string& lang) : ContentDecorator(c), language(lang) {} // constructor
    void ShowDetails() const override { // afisare
        ContentDecorator::ShowDetails();
        std::cout << "Subtitrari: " << language << std::endl;
    }
};

class QualityDecorator : public ContentDecorator { // schimb calitatea imaginii pentru continutul din watchlist
    std::string quality;
public:
    QualityDecorator(std::shared_ptr<Content> c, const std::string& q) : ContentDecorator(c), quality(q) {} // constructor
    void ShowDetails() const override { // afisare
        ContentDecorator::ShowDetails();
        std::cout << "Calitate: " << quality << std::endl;
    }
};

void Menu() {
    std::vector<User> users; // vector de utilizatori
    std::vector<std::shared_ptr<Content>> available_content; // adaug continut

    // filme
    available_content.push_back(std::make_shared<Movie>("Titanic", "Drama/Romance", 1997, 7.8, "James Cameron", 195));
    available_content.push_back(std::make_shared<Movie>("Alladin", "Adventure/Romance/Musical", 2019, 5.1, "Guy Ritchie", 136));
    available_content.push_back(std::make_shared<Movie>("Mufasa: Lion King", "Drama/Adventure/Musical", 2024, 5.1, "Barry Jenkins", 124));
    available_content.push_back(std::make_shared<Movie>("Countdown", "Horror", 2019, 6.6, "Justin Dec", 90));
    available_content.push_back(std::make_shared<Movie>("The Life List", "Drama/Romance", 2025, 7.6, "Adam Brooks", 125));

    // seriale
    available_content.push_back(std::make_shared<TVseries>("Stranger Things", "Horror/Scie-Fi", 2016, 8.7, 34));
    available_content.push_back(std::make_shared<TVseries>("The Rookie", "Drama", 2018, 8.0, 124));
    available_content.push_back(std::make_shared<TVseries>("Gilmore Girls", "Drama/Comedy", 2000, 8.2, 153));
    available_content.push_back(std::make_shared<TVseries>("Grey`s Anatomy", "Drama", 2005, 7.6, 445));
    available_content.push_back(std::make_shared<TVseries>("Arrow", "Action/Drama/Superhero", 2012, 7.5, 170));
    available_content.push_back(std::make_shared<TVseries>("Criminal Minds", "Police procedural/Thriller/Crime drama", 2022, 8.1, 344));
    available_content.push_back(std::make_shared<TVseries>("The fosters", "Family drama", 2013, 7.9, 104));

    // serii de filme
    available_content.push_back(std::make_shared<MovieSeries>("Harry Potter", "Fantasy/Adventure", 120, 2001, 8.1, "Chris Columbus", 8));
    available_content.push_back(std::make_shared<MovieSeries>("Avengers", "Action/Comedy/Superhero", 143, 2012, 8.1, "Joss Whedon", 4));
    available_content.push_back(std::make_shared<MovieSeries>("Scary Movie", "Parody/Comedy", 88, 2000, 6.3, "Keenen Ivory Wayans", 5));
    available_content.push_back(std::make_shared<MovieSeries>("Maleficent", "Fantasy/Adventure/Family/Romance", 120, 2019, 6.6, "Joachim Ronning", 2));
    available_content.push_back(std::make_shared<MovieSeries>("Friday The 13th", "Horror", 95, 1980, 7.6, "Sean S. Cunningham", 6));

    Filter<Content> filter(available_content); // pentru continutul filtrat
    //Playlist<Content> playlist("Favorite");

    User* current_user = nullptr; // retin utilizatorul curent pentru a-i putea afisa watchlist-ul corespunzator

    int choice;
    do {
        std::cout << "\n--- MENIU ---\n";
        std::cout << "1. Creare cont\n";
        std::cout << "2. Autentificare\n";
        std::cout << "3. Adauga continut in watchlist\n";
        std::cout << "4. Vizualizare watchlist\n";
        std::cout << "5. Vizualizare abonament\n";
        std::cout << "6. Recomndare de continut\n";
        std::cout << "7. Vizualizeaza continutul cu cel mai bun rating\n";
        std::cout << "8. Afiseaza detaliile unui continut\n";
        std::cout << "9. Filtreaza dupa gen\n";
        std::cout << "10. Filtreaza dupa rating\n";
        std::cout << "11. Abneaza-te la serial\n";
        std::cout << "12. Adauga subtitrari unui continut\n";
        std::cout << "13. Schimba calitatea unui continut\n";
        std::cout << "14. Gestionare playlists\n";
        std::cout << "15. Deconectare\n";
        std::cout << "0. Iesire\nAlegeti o optiune: ";
        std::cin >> choice;

        if (std::cin.fail()) { // daca a esuat citirea(utilizatorul a introdus litere)
            std::cin.clear(); // reseteaza cin pentru a putea citi din nou
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');// ignora toate caracterele pana laenter (previne bucle infinite)
            std::cout << "Input invalid!\n";
            continue; // incepe o noua iteratie1

        }
        try {
            switch (choice) {
                case 1: {
                    if (current_user) { // utilizatorul trebuie sa nu fie conectat la niciun alt cont in momentul in care isi creaza un cont
                        throw std::runtime_error("Deconectati-va inainte de a crea un nou cont!");
                    }
                    User nou; // citesc utilizatorul nou
                    std::cin >> nou;
                    int type; // utilizatorul isi alege tipul de abonament dorit
                    std::cout << "Ce tip de abonament doriti? (1 - Standard (29.99 RON); 2 - Premium (49.99 RON)): ";
                    std::cin >> type;
                    if (type == 1) {
                        nou.SetSubscript(SubcriptionManager::GetBasicSubscription()); // ii atribui abonament standard(dar creez o singura instanta pentru toti utilizatorii)
                    }
                    else if (type == 2) {
                        nou.SetSubscript(SubcriptionManager::GetPremiumSubscription()); // ii atribui abonament premium(dar creez o singura instanta pentru toti utilizatorii)
                    }
                    else {
                        throw std::invalid_argument("Optiune invalida"); // daca utilizatorul introduce altceva inafara de cele 2 optiuni
                    }
                    users.push_back(nou); // adaug utilizatorul nou in vector
                    std::cout << "Cont creat cu succes!\n";
                    current_user = &nou;
                    break;
                }
                case 2: {
                    if (current_user) { // utilizatorul se poate autentifica doar daca nu e deja conectat la un alt cont
                        throw std::runtime_error("Sunteti deja conectat!");
                    }
                    // citesc datele utilizatorului
                    std::string email;
                    std::cout << "Introduceti email: ";
                    std::cin >> email;
                    std::string password;
                    std::cout << "Introduceti parola: ";
                    std::cin >> password;
                    bool exists = false;
                    for (auto& u : users) { // verific daca utilizatorul introdus se afla in vectorul de utilizatori
                        if (u.login(email, password)) {
                            current_user = &u;
                            exists = true;
                            std::cout << "Autentificare reusita!\n";
                            break;
                        }
                    }
                    if (!exists) { // emailul introdus nu este asociat niciunui cont din vector
                        throw std::runtime_error("Email sau parola incorecte!");
                    }
                    break;
                }
                case 3: {
                    if (!current_user) { // verific daca utilizatorul este conectat in cont
                        throw std::runtime_error("Autentificati-va intai!");
                    }
                    std::cout << "Selectati continutul dorit (0 - " << available_content.size() - 1 << "):\n";
                    for (size_t i = 0; i < available_content.size(); i++) {
                        std::cout << i << ". " << available_content[i]->GetTitle() << "\n"; // ii afisez toate titlurile disponibile pe care le poate introduce in watchlist
                    }
                    int index;
                    std::cin >> index;
                    if (index >= 0 && index < available_content.size()) { // verific daca input ul este valid
                        //watchlists[current_user->GetEmail()].push_back(available_content[index]);
                        current_user->add_to_watchlist(available_content[index]); // adaug continutul in watchlist-ul corespunzator utilizatorului conectat
                        std::cout << "Continut adaugat cu succes!\n";
                    }
                    else {
                        throw std::runtime_error("Continut inexistent!"); // nu gasesc titlul in vectorul de continut disponibil
                    }
                    break;
                }
                case 4: {
                    if (!current_user) { // nu pot verifica continutul din watchlist fara un user conectat
                        throw std::runtime_error("Autentificati-va intai!");
                    }
                    current_user->show_watchlist(); // afisez watchlist-ul utilizatorului conectat
                    break;
                }
                case 5: {
                    if (!current_user) { // nu pot afisa detalii despre abonament daca nu e niciun user conectat
                        throw std::runtime_error("Autentificati-va intai!");
                    }
                    current_user->show_subcription(); // afisez detalii despre abonamentul utilizatorului conectat
                    break;
                }
                case 6: {
                    if (available_content.empty()) { // verific daca am continut in vector
                        throw std::runtime_error("Nu exista continut disponibil!");
                    }
                    std::srand(std::time(nullptr)); // generarea de numere aleatorii este diferita la fiecare rulare
                    int index = std::rand() % available_content.size(); // generez un numar aleator de la 0 la dimensiunea vectorului
                    std::cout << "Recomandare:\n";
                    available_content[index]->ShowDetails();
                    break;
                }
                case 7: {
                    if (available_content.empty()) { // verific daca am continut in vector
                        throw std::runtime_error("Nu exista continut disponibil!");
                    }
                    std::sort(available_content.begin(), available_content.end()); // sortez vectorul de continut disponibil dupa rating
                    std::cout << "Continut cu rating maxim:\n";
                    available_content[available_content.size() - 1]->ShowDetails(); // iau continutul de rating maxim (ultimul din vector)
                    break;
                }
                case 8: {
                    std::string title; // citesc titlul cautat de utilizator
                    std::cout << "Introduceti titlul: ";
                    std::cin >> title;
                    bool exists = false;
                    for (auto& c : available_content) { // caut titlul in vector
                        if (c->GetTitle() == title) {
                            exists = true; // l-am gasit
                            c->ShowDetails();
                            break;
                        }
                    }
                    if (!exists) { // nu l-am gasit
                        std::cout << "Continut inexistent.\n";
                    }
                    break;
                }
                case 9: {
                    // citesc genul cautat de utilizator
                    std::string genre;
                    std::cout << "Introduceti genul cautat: ";
                    std::cin >> genre;
                    auto result = filter.ByGenre(genre); // filtrez dupa gen continutul disponibil
                    if (result.empty()) { // verific daca se potriveste vreun titlu
                        std::cout << "Niciun continut nu se potriveste cu genul cautat!\n";
                        break;
                    }
                    for (auto& c : result) { // afisez rezultatele gasite
                        c->ShowDetails();
                    }
                    break;
                }
                case 10: {
                    // citesc ratingul minim pe care il cauta utilizatorul
                    double rating;
                    std::cout << "Introduceti ratingul minim pe care doriti sa il aiba continutul cautat: ";
                    std::cin >> rating;
                    if (rating < 0 || rating > 10) { // verific daca input ul introdus e valid
                        throw std::out_of_range("Rating invalid!");
                    }
                    auto result = filter.ByRating(rating); // pun rezultatele filtrarii intr-un vector
                    if (result.empty()) { // verific daca am gasit ceva
                        std::cout << "Niciun continut nu se potriveste cu ratingul minim setat!\n";
                        break;
                    }
                    for (auto& c : result) { // afisez rezultatele
                        c->ShowDetails();
                    }
                    break;
                }
                case 11: {
                    if (!current_user) { // un utilizator nu se poate abona la un serial fara sa fie conectat in cont
                        throw std::runtime_error("Autentificati-va intai!");
                    }
                    int index;
                    // afisez serialele la care se poate abona userul
                    std::cout << "Introduceti indexul seriei tv la care doriti sa va abonati:\n";
                    for (int i = 5; i <= 11; i++) {
                        std::cout << i-5 << ". " << available_content[i]->GetTitle() << "\n";
                    }
                    std::cin >> index;
                    if ((index+5) >= 5 && (index+5) <= 11) {
                        auto series = std::dynamic_pointer_cast<TVseries>(available_content[index+5]);
                        series->AddObserver(std::make_shared<UserObserver>(current_user)); // adaug un nou observer pentru serialul selectat
                        std::cout << "V-ati abonat cu succes la serialul " << available_content[index+5]->GetTitle() << "\n";
                        break;
                    }
                    else { // input ul citit este invalid
                        throw std::out_of_range("Index invalid!");
                    }
                }
                case 12: {
                    if (!current_user || current_user->GetWatchlist().empty()) { // utilizatorul nu este conectat sau watchlist ul lui este gol
                        throw std::runtime_error("Autentificati-va intai si/sau adaugati continut in watchlist!");
                    }
                    // ii afisez watchlist ul si il las sa selecteze continutul la care vrea sa adauge subtitrari
                    std::cout << "Selecteaza un film din watchlist (0 - " << current_user->GetWatchlist().size() - 1 << "):\n";
                    current_user->show_watchlist();
                    int index;
                    std::cin >> index;
                    if (index >= 0 && index < current_user->GetWatchlist().size()) {
                        // il las sa aleaga in ce limba vrea subtitrarile
                        std::cout << "Introdu limba subtitrarilor (Romana, Engleza, Franceza, Italiana, Spaniola): ";
                        std::string lang;
                        std::cin.ignore();
                        std::getline(std::cin, lang);
                        // verific daca input ul e valid
                        if (lang == "romana" || lang == "Romana" || lang == "engleza" || lang == "Engleza" || lang == "franceza" || lang == "Franceza" || lang == "italiana" || lang == "Italiana" || lang == "spaniola" || lang == "Spaniola") {
                            auto original_content = current_user->GetWatchlist()[index];
                            auto decorated_content = std::make_shared<SubtitleDecorator>(original_content, lang); // adaug subtitrarile / decorez continutul
                            current_user->update_watchlist(index, decorated_content); // actualizez continutul din watchlist pentru a afisa continutul modificat
                            std::cout << "Subtitrari adaugate cu succes!\n";
                            break;
                        }
                        else { // input invalid
                            std::cout << "Optiune invalida!";
                            break;
                        }
                    }
                    else {
                        throw std::out_of_range("Index invalid!");
                    }
                }
                case 13: {
                    if (!current_user || current_user->GetWatchlist().empty()) { // utilizatorul nu este conectat sau watchlist ul lui este gol
                        throw std::runtime_error("Autentificati-va intai si/sau adaugati continut in watchlist!");
                    }
                    // ii afisez watchlist ul si il las sa selecteze continutul la care vrea sa schimbe calitatea imaginii
                    std::cout << "Selecteaza un film din watchlist (0 - " << current_user->GetWatchlist().size() - 1 << "):\n";
                    current_user->show_watchlist();
                    int index;
                    std::cin >> index;
                    if (index >= 0 && index < current_user->GetWatchlist().size()) { // input valid
                        std::cout << "Alege calitatea ( 1-HD, 2-4K): "; // alege ce calitate vrea
                        int quality_choice;
                        std::cin >> quality_choice;
                        if (quality_choice !=1 && quality_choice != 2) { // input valid
                            throw std::runtime_error("Optiune invalida!");
                        }
                        std::string quality = (quality_choice == 1) ? "HD" : "4K"; // convertesc inputul la string in functie de optiunea selectata
                        auto original_content = current_user->GetWatchlist()[index];
                        auto decorated_content = std::make_shared<QualityDecorator>(original_content, quality); // decorez continutul / schimb calitatea imaginii
                        current_user->update_watchlist(index, decorated_content); // actualizez continutul in watchlist
                        std::cout << "Calitate schimbata in " << quality << "!\n";
                        break;
                    }
                    else {
                        throw std::out_of_range("Index invalid!");
                    }
                }
                case 14: {
                    if (!current_user) { // un utilizator neconectat nu isi poate accesa playlist urile
                        throw std::runtime_error("Autentificati-va intai!");
                    }
                    std::cout << "\n--- PLAYLIST URI ---\n";
                    std::cout << "1. Creaza playlist nou\n";
                    std::cout << "2. Adauga continut in playlist existent\n";
                    std::cout << "3. Reda playlist\n";
                    std::cout << "4. Afiseaza playlist uri\n";
                    std::cout << "Alegeti o optiune: ";
                    int playlist_choice;
                    std::cin >> playlist_choice;
                    switch (playlist_choice) {
                        case 1: {
                            std::cout << "Nume playlist: ";
                            std::string name; // setez numele playlist ului
                            std::cin.ignore();
                            std::getline(std::cin, name);
                            current_user->CreatePlaylist(name); // creez playlist ul
                            std::cout << "Playlist " << name << " creat cu succes!\n";
                            break;
                        }
                        case 2: {
                            if (current_user->GetPlaylist().empty()) { // utilizatorul curent nu are niciun playlist creat
                                throw std::runtime_error("Nu aveti niciun playlist!");
                            }
                            // il las sa isi aleaga playlist ul in care vrea sa adauge continut
                            std::cout << "Alege playlist:\n";
                            current_user->ShowPlaylists();
                            int playlist_index;
                            std::cin >> playlist_index;
                            if (playlist_index >=0 && playlist_index < current_user->GetPlaylist().size()) { // input valid
                                std::cout << "Selectati continutul dorit (0 - " << available_content.size() - 1 << "):\n";
                                for (size_t i = 0; i < available_content.size(); i++) {
                                    std::cout << i << ". " << available_content[i]->GetTitle() << "\n"; // ii afisez toate titlurile disponibile pe care le poate introduce in watchlist
                                }
                                int content_index;
                                std::cin >> content_index;
                                if (content_index >=0 && content_index < available_content.size()) { // input valid
                                    current_user->AddToPlaylist(playlist_index, available_content[content_index]); // adaug continut in playlist ul selectat
                                    std::cout << "Continut adaugat cu succes in playlist!\n";
                                    break;
                                }
                                else {
                                    throw std::runtime_error("Optiune invalida!"); // input invalid
                                }
                            }
                            else {
                                throw std::runtime_error("Optiune invalida!"); // input ul introdus nu era valid
                            }
                        }
                        case 3: {
                            std::cout << "Alege playlist ul pe care vrei sa il redai:\n";
                            // il las sa isi aleaga playlist ul pe care vrea sa il redea
                            current_user->ShowPlaylists();
                            int playlist_index;
                            std::cin >> playlist_index;
                            if (playlist_index >=0 && playlist_index < current_user->GetPlaylist().size()) { // input valid
                                current_user->PlayPlaylist(playlist_index); // redau playlist ul selectat
                            }
                            else {
                                throw std::runtime_error("Optiune invalida!"); // input invalid
                            }
                        }
                        case 4: {
                            if (current_user->GetPlaylist().empty()) { // utilizatorul nu are playlist uri
                                std::cout << "Nu aveti niciun playlist!\n";
                                break;
                            }
                            std::cout << "Playlist urile tale:\n";
                            current_user->ShowPlaylists(); // afisez toate playlist urile utilizatorului curent
                            break;
                        }
                        default: {
                            throw std::runtime_error("Optiune invalida!"); // optiunea selectat nu era valida
                        }
                    }
                    break;
                }
                case 15: {
                    if (!current_user) { // utilizatorul nu se poate deconecta daca nu era conectat la niciun cont
                        throw std::runtime_error("Nu sunteti conectat la niciun cont!");
                    }
                    current_user = nullptr; // setez pointerul de utilizator curent sa nu mai pointeze la nimic
                    std::cout << "Utlizator deconectat cu succes!\n";
                    break;
                }
                case 0: {
                    std::cout << "Iesire...\n"; // ies din program
                    break;
                }
                default:
                    std::cout << "Optiune invalida.\n"; // input invalid
            }
        } catch (std::exception& e) {
            std::cout << "Eroare: " << e.what() << '\n'; // eroare (input invalid)
        }
    } while (choice != 0);
}
int main() {
    Menu();
    return 0;
}