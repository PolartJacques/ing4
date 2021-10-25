-- ------------------------------------------------------
-- NOTE: DO NOT REMOVE OR ALTER ANY LINE FROM THIS SCRIPT
-- ------------------------------------------------------

select 'Query 00' as '';
-- Show execution context
select current_date(), current_time(), user(), database();
-- Conform to standard group by constructs
set session sql_mode = 'ONLY_FULL_GROUP_BY';

-- Write the SQL queries that return the information below:
-- Ecrire les requêtes SQL retournant les informations ci-dessous:

select 'Query 01' as '';
-- The countries of residence the supplier had to ship products to in 2014
-- Les pays de résidence où le fournisseur a dû envoyer des produits en 2014
SELECT residence FROM customers JOIN orders ON customers.cid = orders.cid
WHERE YEAR(orders.odate) = 2014 AND customers.residence IS NOT NULL
GROUP BY residence;


select 'Query 02' as '';
-- For each known country of origin, its name, the number of products from that country, their lowest price, their highest price
-- Pour chaque pays d'orgine connu, son nom, le nombre de produits de ce pays, leur plus bas prix, leur plus haut prix
SELECT origin, SUM(quantity) AS total, MIN(price) as min_price, MAX(price) as max_price
FROM (SELECT origin, quantity, price FROM orders JOIN products ON orders.pid = products.pid) AS custom_table
WHERE origin IS NOT NULL
GROUP BY origin;


select 'Query 03' as '';
-- The customers who ordered in 2014 all the products (at least) that the customers named 'Smith' ordered in 2013
-- Les clients ayant commandé en 2014 tous les produits (au moins) commandés par les clients nommés 'Smith' en 2013
SELECT * FROM customers c
WHERE NOT EXISTS (
    SELECT pid FROM customers NATURAL JOIN orders
    WHERE cname = 'Smith'
    AND YEAR(odate) = 2013
    AND pid NOT IN (
        SELECT DISTINCT pid from orders
        WHERE YEAR(odate) = 2014
        AND cid = c.cid
    )
);


select 'Query 04' as '';
-- For each customer and each product, the customer's name, the product's name, the total amount ordered by the customer for that product,
-- sorted by customer name (alphabetical order), then by total amount ordered (highest value first), then by product id (ascending order)
-- Par client et par produit, le nom du client, le nom du produit, le montant total de ce produit commandé par le client, 
-- trié par nom de client (ordre alphabétique), puis par montant total commandé (plus grance valeur d'abord), puis par id de produit (croissant)
SELECT cname, pname, SUM(quantity * price) AS total
FROM customers c
NATURAL JOIN products p
NATURAL JOIN orders o
GROUP BY cname, pname, pid
ORDER BY cname, total DESC, pid;


select 'Query 05' as '';
-- The customers who only ordered products originating from their country
-- Les clients n'ayant commandé que des produits provenant de leur pays
SELECT *
FROM customers c
WHERE not EXISTS (
    SELECT origin
    FROM orders
    NATURAL JOIN products
    WHERE c.residence != origin
)
AND c.residence IS NOT NULL;

select 'Query 06' as '';
-- The customers who ordered only products originating from foreign countries 
-- Les clients n'ayant commandé que des produits provenant de pays étrangers
SELECT *
FROM customers c
WHERE not EXISTS (
    SELECT origin
    FROM orders
    NATURAL JOIN products
    WHERE c.residence = origin
)
AND c.residence IS NOT NULL;


select 'Query 07' as '';
-- The difference between 'USA' residents' per-order average quantity and 'France' residents' (USA - France)
-- La différence entre quantité moyenne par commande des clients résidant aux 'USA' et celle des clients résidant en 'France' (USA - France)
SELECT (usa.avg - france.avg) difference
FROM (
    SELECT AVG(quantity) avg
    FROM customers
    NATURAL JOIN orders
    WHERE residence = 'USA'
) usa
CROSS JOIN (
    SELECT AVG(quantity) avg
    FROM customers
    NATURAL JOIN orders
    WHERE residence = 'France'
) france;


select 'Query 08' as '';
-- The products ordered throughout 2014, i.e. ordered each month of that year
-- Les produits commandés tout au long de 2014, i.e. commandés chaque mois de cette année
SELECT pid, pname, price, origin
FROM (
    SELECT pid, pname, price, origin, COUNT(mnth) cnt
    FROM (
        SELECT pid, pname, price, origin, MONTH(odate) mnth
        FROM orders
        NATURAL JOIN products
        WHERE YEAR(odate) = 2014
        GROUP BY pid, pname, price, origin, mnth
    ) t1
    GROUP BY pid, pname, price, origin
)t2
WHERE cnt = 12;


select 'Query 09' as '';
-- The customers who ordered all the products that cost less than $5
-- Les clients ayant commandé tous les produits de moins de $5
SELECT *
FROM customers c
WHERE NOT EXISTS (
    SELECT pid
    FROM products p
    WHERE p.price < 5
    AND p.pid NOT IN (
        SELECT pid
        FROM orders o
        WHERE o.cid = c.cid
    )
);


select 'Query 10' as '';
-- The customers who ordered the greatest number of common products. Display 3 columns: cname1, cname2, number of common products, with cname1 < cname2
-- Les clients ayant commandé le grand nombre de produits commums. Afficher 3 colonnes : cname1, cname2, nombre de produits communs, avec cname1 < cname2
SELECT cname1, cname2, COUNT(pid)common
FROM (
    SELECT cid1, cname1, cid2, cname2, pid1 pid
    FROM (
        SELECT cid cid1, cname cname1, pid pid1
        FROM customers c1
        NATURAL JOIN orders o1
    ) t1
    CROSS JOIN (
        SELECT cid cid2, cname cname2, pid pid2
        FROM customers c2
        NATURAL JOIN orders o2
    )t2
    WHERE cname1 < cname2
    AND pid1 = pid2
    GROUP BY cid1, cid2, pid1
    ORDER BY cname1, cname2
)t3
GROUP BY cid1, cid2
ORDER BY common DESC;


select 'Query 11' as '';
-- The customers who ordered the largest number of products
-- Les clients ayant commandé le plus grand nombre de produits
SELECT *
FROM customers c
NATURAL JOIN (
    SELECT SUM(quantity) ordered, cid
    FROM orders
    GROUP BY cid
) t1
ORDER BY ordered DESC;


select 'Query 12' as '';
-- The products ordered by all the customers living in 'France'
-- Les produits commandés par tous les clients vivant en 'France'
SELECT *
FROM products p
WHERE NOT EXISTS (
    -- lsit des français qui n'ont pas acheté le produit
    SELECT cid
    FROM customers c
    NATURAL JOIN orders o
    WHERE c.residence = 'France'
    AND c.cid NOT IN (
        -- liste des français qui ont acheyé le produit
        SELECT cid
        FROM customers c2
        NATURAL JOIN orders o2
        WHERE c2.residence = 'France'
        AND o2.pid = p.pid
    )
);


select 'Query 13' as '';
-- The customers who live in the same country customers named 'Smith' live in (customers 'Smith' not shown in the result)
-- Les clients résidant dans les mêmes pays que les clients nommés 'Smith' (en excluant les Smith de la liste affichée)
SELECT *
FROM customers c
WHERE c.residence IN (
    -- list des residence des smith
    SELECT residence
    FROM customers c2
    WHERE c2.cname = 'Smith'
)
AND c.cname != 'Smith';


select 'Query 14' as '';
-- The customers who ordered the largest total amount in 2014
-- Les clients ayant commandé pour le plus grand montant total sur 2014
SELECT cid, cname, residence, SUM(price) total_2014
FROM customers c
NATURAL JOIN orders o
NATURAL JOIN products
WHERE YEAR(o.odate) = 2014
GROUP BY c.cid
ORDER BY total_2014 DESC;


select 'Query 15' as '';
-- The products with the largest per-order average amount 
-- Les produits dont le montant moyen par commande est le plus élevé
SELECT pid, pname, price, origin, AVG(quantity) avg_quantity
FROM products p
NATURAL JOIN orders o
GROUP BY p.pid
ORDER BY avg_quantity DESC;


select 'Query 16' as '';
-- The products ordered by the customers living in 'USA'
-- Les produits commandés par les clients résidant aux 'USA'
SELECT pid, pname, price, origin
FROM customers c
NATURAL JOIN orders o
NATURAL JOIN products p
WHERE c.residence = 'USA'
GROUP BY pid, pname, price, origin;


select 'Query 17' as '';
-- The pairs of customers who ordered the same product en 2014, and that product. Display 3 columns: cname1, cname2, pname, with cname1 < cname2
-- Les paires de client ayant commandé le même produit en 2014, et ce produit. Afficher 3 colonnes : cname1, cname2, pname, avec cname1 < cname2
SELECT cname1, cname2, pname1 pname
FROM (
    SELECT cid cid1, cname cname1, pid pid1, pname pname1
    FROM customers c1
    NATURAL JOIN orders o1
    NATURAL JOIN products p1
    WHERE YEAR(o1.odate) = 2014
) t1
CROSS JOIN (
    SELECT cid cid2, cname cname2, pid pid2, pname pname2
    FROM customers c2
    NATURAL JOIN orders o2
    NATURAL JOIN products p2
    WHERE YEAR(o2.odate) = 2014
)t2
WHERE cname1 < cname2
AND pid1 = pid2
GROUP BY cid1, cid2, pid1
ORDER BY cname1, cname2;


select 'Query 18' as '';
-- The products whose price is greater than all products from 'India'
-- Les produits plus chers que tous les produits d'origine 'India
SELECT *
FROM products p1
WHERE NOT EXISTS (
    -- tous les produit indiens plus cher ou au même prix
    SELECT pid
    FROM products p2
    WHERE p2.origin = 'India'
    AND p2.price >= p1.price
);


select 'Query 19' as '';
-- The products ordered by the smallest number of customers (products never ordered are excluded)
-- Les produits commandés par le plus petit nombre de clients (les produits jamais commandés sont exclus)
SELECT pid, pname, price, origin, COUNT(quantity) total
FROM products p
NATURAL JOIN orders o
GROUP BY pid
ORDER BY total;


select 'Query 20' as '';
-- For all countries listed in tables products or customers, including unknown countries: the name of the country, the number of customers living in this country, the number of products originating from that country
-- Pour chaque pays listé dans les tables products ou customers, y compris les pays inconnus : le nom du pays, le nombre de clients résidant dans ce pays, le nombre de produits provenant de ce pays 
SELECT country, COUNT(DISTINCT cid) peoples, COUNT(DISTINCT pid) products
FROM (
    SELECT residence country
    FROM customers
    UNION
    SELECT origin country
    FROM products
    GROUP BY country
) t1
LEFT JOIN customers
ON residence = country OR (residence IS NULL AND country IS NULL)
LEFT JOIN products
ON origin = country OR (origin IS NULL AND country IS NULL)
GROUP BY country;

