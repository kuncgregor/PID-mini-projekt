# Mini projekt "PID kontroler" pri izbernem predmetu RULE FMF

## Kaj je PID 


$$ u(t) = K_p e(t) + K_i \int_{0}^{t} e(\tau) d\tau + K_d \frac{de(t)}{dt} $$

kjer:
- $ u(t) $ je izhod kontrolerja
- $ e(t) $ je napaka (razlika med želeno in dejansko vrednostjo)
- $ K_p $ je proporcionalni koeficient
- $ K_i $ je integralni koeficient
- $ K_d $ je diferencialni koeficient

## Struktura 
- `/CAD` - Vsebuje CAD datoteke za kompunente
- `/src` - Izvorna koda za PID kontroler
- `/docs` - Dodatna dokumentacija (ki jo mogoce vklučim v prihodnosti)

## Oprema
Za ta projekt potrebujete naslednjo strojno opremo:
- Servo motor
- Arduino Uno
- Ultrazvočni senzor

## Navodila
1. Cloneraj repozitorij
2. Preglejte CAD datoteke za mehansko sestavljanje
3. Print in sestava eksperimenta (V PRIHODNOSTI DODAM NAVODILA)
4. Konfigurirajte PID parametre v kodi

## Licenca
Ta projekt je licenciran pod MIT licenco.

