#include <iostream>
#include <list>
#include <queue>
#include <vector>
using namespace std;

template <typename TipKljuca, typename TipVrijednosti> class Mapa {
public:
    Mapa() {}
    virtual ~Mapa() {}
    virtual TipVrijednosti &operator[](TipKljuca k) = 0;
    virtual TipVrijednosti operator[](const TipKljuca k) const = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca &kljuc) = 0;
};

template <typename TipKljuca, typename TipVrijednosti>
class HashMapaLan : public Mapa<TipKljuca, TipVrijednosti> {
    unsigned int (*hashFunkcija)(TipKljuca, unsigned int) = 0;
    int velicina, kapacitet;
    vector<list<pair<TipKljuca, TipVrijednosti>>> hash_map;

public:
    HashMapaLan()
            : velicina(0), kapacitet(2),
              hash_map(kapacitet, list<pair<TipKljuca, TipVrijednosti>>()),
              hashFunkcija(nullptr) {}
    ~HashMapaLan() {}
    void definisiHashFunkciju(unsigned int (*hashFunction)(TipKljuca kljuc,
                                                           unsigned int opseg)) {
        hashFunkcija = hashFunction;
    }
    int brojElemenata() const { return velicina; }
    void obrisi() {
        for (auto &element : hash_map)
            element.clear();
        velicina = 0;
    }
    void obrisi(const TipKljuca &kljuc) {
        if (velicina == 0)
            return;
        if (!hashFunkcija)
            throw logic_error("Nije definisana hash funkcija!");
        int pronadji = hashFunkcija(kljuc, hash_map.size());
        auto it = hash_map[pronadji].begin();
        while (it != hash_map[pronadji].end() && kljuc > it->first) {
            it++;
        }
        if (it == hash_map[pronadji].end() || it->first != kljuc)
            throw logic_error("Trazeni element sa tim kljucem nije pronadjen!");
        auto kopija = it;
        it++;
        hash_map[pronadji].erase(kopija, it);
        velicina--;
    }
    TipVrijednosti &operator[](TipKljuca kljuc) {
        if (!hashFunkcija)
            throw logic_error("Nije definisana hash funkcija!");
        int pronadji = hashFunkcija(kljuc, hash_map.size());
        auto it = hash_map[pronadji].begin();
        TipVrijednosti tp = TipVrijednosti();
        while (it != hash_map[pronadji].end() && kljuc > it->first)
            it++;
        if (it == hash_map[pronadji].end() || it->first != kljuc) {
            it = hash_map[pronadji].insert(
                    it, pair<TipKljuca, TipVrijednosti>(kljuc, tp));
            velicina++;
        }
        return it->second;
    }
    TipVrijednosti operator[](const TipKljuca kljuc) const {
        if (!hashFunkcija)
            throw logic_error("Nije definisana hash funkcija!");
        int pronadji = hashFunkcija(kljuc, hash_map.size());
        auto it = hash_map[pronadji].begin();
        TipVrijednosti tp = TipVrijednosti();
        while (it != hash_map[pronadji].end() && kljuc > it->first)
            it++;
        if (it == hash_map[pronadji].end() || it->first != kljuc)
            tp;
        return it->second;
    }
};

template <typename TipOznake> class Grana;

template <typename TipOznake> class GranaIterator;

template <typename TipOznake> class Cvor;

template <typename TipOznake> class UsmjereniGraf {
public:
    UsmjereniGraf() {}
    UsmjereniGraf(int broj_cvorova) {}
    virtual ~UsmjereniGraf() {}
    virtual int dajBrojCvorova() const = 0;
    virtual void postaviBrojCvorova(int broj_cvorova) = 0;
    virtual void dodajGranu(int polazni_cvor, int dolazni_cvor,
                            float tezina_grane) = 0;
    virtual void obrisiGranu(int polazni_cvor, int dolazni_cvor) = 0;
    virtual void postaviTezinuGrane(int polazni_cvor, int dolazni_cvor,
                                    float tezina_grane = 0) = 0;
    virtual float dajTezinuGrane(int polazni_cvor, int dolazni_cvor) const = 0;
    virtual bool postojiGrana(int polazni_cvor, int dolazni_cvor) = 0;
    virtual void postaviOznakuCvora(int brojCvora, TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuCvora(int brojCvora) const = 0;
    virtual void postaviOznakuGrane(int polazni_cvor, int dolazni_cvor,
                                    TipOznake oznaka) = 0;
    virtual TipOznake dajOznakuGrane(int polazni_cvor,
                                     int dolazni_cvor) const = 0;
    virtual Grana<TipOznake> dajGranu(int polazni_cvor, int dolazni_cvor) = 0;
    virtual Cvor<TipOznake> dajCvor(int cvor) = 0;
    virtual GranaIterator<TipOznake> dajGranePocetak() = 0;
    virtual GranaIterator<TipOznake> dajGraneKraj() = 0;
};

template <typename TipOznake> class Grana {
    UsmjereniGraf<TipOznake> *ug;
    int polazni_cvor, dolazni_cvor;

public:
    Grana(UsmjereniGraf<TipOznake> *dg, int start_vertex, int end_vertex)
            : ug(dg), polazni_cvor(start_vertex), dolazni_cvor(end_vertex) {}
    float dajTezinu() const {
        return ug->dajTezinuGrane(polazni_cvor, dolazni_cvor);
    }
    void postaviTezinu(float tezina_grane) {
        ug->postaviTezinuGrane(polazni_cvor, dolazni_cvor, tezina_grane);
    }
    TipOznake dajOznaku() const {
        return ug->dajOznakuGrane(polazni_cvor, dolazni_cvor);
    }
    void postaviOznaku(TipOznake oznaka) {
        ug->postaviOznakuGrane(polazni_cvor, dolazni_cvor, oznaka);
    }
    Cvor<TipOznake> dajPolazniCvor() const { return ug->dajCvor(polazni_cvor); }
    Cvor<TipOznake> dajDolazniCvor() const { return ug->dajCvor(dolazni_cvor); }
};

template <typename TipOznake> class Cvor {
    UsmjereniGraf<TipOznake> *ug;
    int broj_cvora;

public:
    Cvor(UsmjereniGraf<TipOznake> *dg, int number_of_vertex)
            : ug(dg), broj_cvora(number_of_vertex) {}
    TipOznake dajOznaku() const { return ug->dajOznakuCvora(broj_cvora); }
    void postaviOznaku(TipOznake oznaka) {
        ug->postaviOznakuCvora(broj_cvora, oznaka);
    }
    int dajRedniBroj() const { return broj_cvora; }
};

template <typename TipOznake> class GranaIterator {
    UsmjereniGraf<TipOznake> *ug;
    int polazni_cvor, dolazni_cvor;
    void inkrementuj(int &polazni_cvor, int &dolazni_cvor) {
        dolazni_cvor + 1 >= ug->dajBrojCvorova() ? dolazni_cvor = 0,
                polazni_cvor++ : dolazni_cvor++;
    }

public:
    GranaIterator(UsmjereniGraf<TipOznake> *dg, int start_vertex, int end_vertex)
            : ug(dg), polazni_cvor(start_vertex), dolazni_cvor(end_vertex) {}
    Grana<TipOznake> operator*() {
        return ug->dajGranu(polazni_cvor, dolazni_cvor);
    }
    bool operator==(const GranaIterator &iter) const {
        if (iter.ug == ug && iter.polazni_cvor == polazni_cvor &&
            iter.dolazni_cvor == dolazni_cvor)
            return true;
        return false;
    }
    bool operator!=(const GranaIterator &iter) const {
        if (iter.ug == ug && iter.polazni_cvor == polazni_cvor &&
            iter.dolazni_cvor == dolazni_cvor)
            return false;
        return true;
    }
    GranaIterator &operator++() {
        if (dolazni_cvor + 1 >= ug->dajBrojCvorova()) {
            dolazni_cvor = 0;
            polazni_cvor++;
        } else
            dolazni_cvor++;
        while (polazni_cvor < ug->dajBrojCvorova() &&
               !ug->postojiGrana(polazni_cvor, dolazni_cvor)) {
            if (dolazni_cvor + 1 >= ug->dajBrojCvorova()) {
                dolazni_cvor = 0;
                polazni_cvor++;
            } else
                dolazni_cvor++;
        }
        return *this;
    }
    GranaIterator operator++(int) {
        GranaIterator<TipOznake> temp = (*this);
        ++(*this);
        return temp;
    }
};

template <typename TipOznake> struct Element {
    float tezina;
    int vrijednost;
    TipOznake oznaka;
    Element(int vertex, float weight = 0, TipOznake oznaka = TipOznake())
            : vrijednost(vertex), tezina(weight), oznaka(oznaka) {}
};
template <typename TipOznake>
class ListaGraf : public UsmjereniGraf<TipOznake> {
    vector<list<Element<TipOznake>>> elementi;
    vector<TipOznake> cvorovi;

public:
    ListaGraf() {}
    ListaGraf(int n) : UsmjereniGraf<TipOznake>(n) {
        if (n < 0)
            throw logic_error(
                    "Lista susjedstva ne moze imati negativan broj elemenata!");
        elementi.resize(n);
        cvorovi.resize(n);
    }
    ~ListaGraf() {}
    void postaviBrojCvorova(int broj_cvorova) {
        if (broj_cvorova < cvorovi.size())
            throw logic_error("Broj cvorova ne moze biti manji od prethodnog!");
        cvorovi.resize(broj_cvorova);
        elementi.resize(broj_cvorova);
    }
    int dajBrojCvorova() const { return elementi.size(); }
    TipOznake dajOznakuCvora(int broj_cvora) const { return cvorovi[broj_cvora]; }
    void postaviOznakuCvora(int broj_cvora, TipOznake oznaka) {
        cvorovi[broj_cvora] = oznaka;
    }
    Cvor<TipOznake> dajCvor(int broj_cvora) {
        return Cvor<TipOznake>(this, broj_cvora);
    }

    typename list<Element<TipOznake>>::iterator trazeni(int polazni_cvor,
                                                        int dolazni_cvor) {
        typename list<Element<TipOznake>>::iterator it;
        for (it = elementi[polazni_cvor].begin();
             it != elementi[polazni_cvor].end(); it++) {
            if (it->vrijednost == dolazni_cvor)
                return it;
            else if (it->vrijednost > dolazni_cvor)
                return elementi[polazni_cvor].end();
        }
        return it;
    }
    typename list<Element<TipOznake>>::const_iterator
    trazeni(int polazni_cvor, int dolazni_cvor) const {
        typename list<Element<TipOznake>>::const_iterator it;
        for (it = elementi[polazni_cvor].begin();
             it != elementi[polazni_cvor].end(); it++) {
            if (it->vrijednost == dolazni_cvor)
                return it;
            else if (it->vrijednost > dolazni_cvor)
                return elementi[polazni_cvor].end();
        }
        return it;
    }
    void dodajGranu(int polazni_cvor, int dolazni_cvor, float tezina_grane) {
        auto it = elementi[polazni_cvor].begin();
        while (it != elementi[polazni_cvor].end() && dolazni_cvor > it->vrijednost)
            it++;
        elementi[polazni_cvor].insert(
                it, Element<TipOznake>(dolazni_cvor, tezina_grane));
    }
    Grana<TipOznake> dajGranu(int polazni_cvor, int dolazni_cvor) {
        return Grana<TipOznake>(this, polazni_cvor, dolazni_cvor);
    }
    TipOznake dajOznakuGrane(int polazni_cvor, int dolazni_cvor) const {
        return trazeni(polazni_cvor, dolazni_cvor)->oznaka;
    }
    void postaviTezinuGrane(int polazni_cvor, int dolazni_cvor,
                            float tezina_grane) {
        trazeni(polazni_cvor, dolazni_cvor)->tezina = tezina_grane;
    }
    float dajTezinuGrane(int polazni_cvor, int dolazni_cvor) const {
        return trazeni(polazni_cvor, dolazni_cvor)->tezina;
    }
    void obrisiGranu(int polazni_cvor, int dolazni_cvor) {
        elementi[polazni_cvor].erase(trazeni(polazni_cvor, dolazni_cvor));
    }
    void postaviOznakuGrane(int polazni_cvor, int dolazni_cvor,
                            TipOznake oznaka) {
        trazeni(polazni_cvor, dolazni_cvor)->oznaka = oznaka;
    }
    bool postojiGrana(int polazni_cvor, int dolazni_cvor) {
        return trazeni(polazni_cvor, dolazni_cvor) != elementi[polazni_cvor].end();
    }
    GranaIterator<TipOznake> dajGranePocetak() {
        GranaIterator<TipOznake> grana(this, 0, -1);
        return ++grana;
    }
    GranaIterator<TipOznake> dajGraneKraj() {
        GranaIterator<TipOznake> grana(this, dajBrojCvorova(), 0);
        return grana;
    }
};

template <typename TipOznake>
void bfs(UsmjereniGraf<TipOznake> *ug, vector<Cvor<TipOznake>> &breadth,
         Cvor<TipOznake> cvor) {
    cvor.postaviOznaku(1);
    breadth.push_back(cvor);
    queue<Cvor<TipOznake>> q;
    q.push(cvor);
    while (!q.empty()) {
        Cvor<TipOznake> c = q.front();
        q.pop();
        GranaIterator<TipOznake> grana = ug->dajGranePocetak();
        for (auto pom = grana; pom != ug->dajGraneKraj(); pom++) {
            auto start = (*pom).dajPolazniCvor();
            auto end = (*pom).dajDolazniCvor();
            if (end.dajOznaku() != 1 && start.dajRedniBroj() == c.dajRedniBroj()) {
                end.postaviOznaku(1);
                breadth.push_back(end);
                q.push(end);
            }
        }
    }
}

template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake> *ug, vector<Cvor<TipOznake>> &depth,
         Cvor<TipOznake> cvor) {
    cvor.postaviOznaku(1);
    depth.push_back(cvor);
    GranaIterator<TipOznake> grana = ug->dajGranePocetak();
    GranaIterator<TipOznake> breach = grana;
    while (breach != ug->dajGraneKraj()) {
        Cvor<TipOznake> pom = (*breach).dajDolazniCvor();
        if ((*breach).dajPolazniCvor().dajRedniBroj() == cvor.dajRedniBroj() &&
            pom.dajOznaku() != 1)
            dfs(ug, depth, pom);
        breach++;
    }
}
int main() { return 0; }