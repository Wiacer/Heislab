#set text(lang: "nb")
#show heading.where(level: 2): set text(size: 12pt) 


#include "forside.typ"

#set page(columns: 2,
    footer: context{
        let p = counter(page).get().at(0)
        let total = counter(page).final().at(0)
        
        if p > 1{
            align(center)[side #(p - 1) av #(total - 1)]
        }
    }
)

#include "innledning.typ"

\ 

#include "fremgangsmåte.typ"

\

#include "struktur.typ"

\

#include "refleksjon2.typ"

#show link: underline
#show link: set text(blue)


Link til github repoet vårt: \ 
#link("https://github.com/Wiacer/Heislab")\
Merk at det vil være commits utenom oss fra github profiler som var sist pålogget på heislabben (vi gadd/turte ikke logge inn med vår egen konto hver gang)

#set page(columns: 1)
