# Как добавить новую доп. профессию

Весь код, описывающий дополнительные профессии, находится в плагине [craft](https://github.com/dreamland-mud/dreamland_code/tree/master/plug-ins/craft).


## Добавьте конфигурацию для новой профессии

Профайлы, описывающие дополнительные профессии, находятся в каталоге [craft-professions](https://github.com/dreamland-mud/dreamland_world/tree/master/craft-professions).

* В заголовке указано имя класса, реализующее данную профессию. По умолчанию это [CraftProfession](https://github.com/dreamland-mud/dreamland_code/tree/master/plug-ins/craft/subprofession.h). 
```xml
<CraftProfession type="CraftProfession">
```

* Внутри элемента ```help``` содержится справка по этой профессии. Аттрибут ```keyword``` содержит необязательные ключевые слова, по которым дополнительно должна находиться эта статья справки:
```xml
 <help level="-1" type="CraftProfessionHelp" keyword="TATTOO ТАТУИРОВКА">
```

* Опишите имена с падежами, в единственном (```rusName```) и множественном (```mltName```) числе.
* Элемент ```baseExp``` содержит базовый опыт на уровень.
* Элемент ```maxLevel``` содержит, сколько всего уровней планируется в этой профессии.

После этого в мире наберите:
```
plug reload craft
```
и новая профессия станет видна в справке, а также доступна через Феню (читать ниже).

## Проверьте, что профессия добавлена корректно

В корневом объекте есть метод ```.CraftProfession()```, создающий враппер вокруг доп. профессии и дающий доступ к ее полям и методам. Используйте метод ```api()``` для просмотра полного списка полей и методов:
```
eval prof=.CraftProfession("tattooist")
eval ptc(prof.nameMlt)
eval ptc(prof.api())
```

## Потестируйте набор опыта

Можно установить себе или тестовому персонажу 1й уровень новой профессии:
```
eval prof.setLevel(this, 1)
```
Затем набрать немного опыта:
```
eval prof.gainExp(this, 100)
```
И достичь следующего уровня мастерства в профессии, набрав ровно столько опыта, сколько оставалось до следующего уровня:
```
eval tnl=prof.getExpToLevel(ch)
eval prof.gainExp(this, tnl)
```

## Добавьте сценарий на Фене

Для того, чтобы новая профессия стала доступна для всех, надо придумать и реализовать способ, которым персонажи смогут достичь 1го уровня мастерства. В случае татуировщика за это отвечает [speech-прога](https://github.com/dreamland-mud/dreamland_world/blob/master/fenia.local/t1.f%2B%2B) на мобе, реагирующая на фразу "хочу стать татуировщиком". 

Другие примеры могут включать манипуляции с предметами или результат выполнения небольшого квеста.



