USE test;

DROP TABLE IF EXISTS A;
DROP TABLE IF EXISTS B;

CREATE TABLE A(k UInt32, t DateTime64, a Float64) ENGINE = MergeTree() ORDER BY (k, t);
INSERT INTO A(k,t,a) VALUES (2,1,1),(2,3,3),(2,5,5);

CREATE TABLE B(k UInt32, t DateTime64, b Float64) ENGINE = MergeTree() ORDER BY (k, t);
INSERT INTO B(k,t,b) VALUES (2,2,3);

SELECT k, t, a, b FROM A ASOF LEFT JOIN B USING(k,t) ORDER BY (k,t);

DROP TABLE B;
DROP TABLE A;
