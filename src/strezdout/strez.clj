(ns strezdout.strez
  (:require [clojure.string :as str]))

(defn split-by-pred [pred coll] [(take-while pred coll) (drop-while pred coll)])

(let [matcher (complement (partial = "TESTLOLKEKTEST"))]
  (defn read-test "Read single test from sequence of lines" [lines]
    (let [[test lines'] (split-by-pred matcher lines)] [test (next lines')])))
(defn read-tests "Read all the tests from given lines (should be lines of a single category)"
  ([lines] (if lines (let [[test lines'] (read-test lines)]
                       (lazy-seq (cons test (read-tests lines')))))))

; read single category with its tests, return it and rest of lines
(let [delim "TESTCATEGORY"
      name-index (count (str delim " "))
      matcher (complement (partial re-find #"^TESTCATEGORY\s.*$"))]
  (defn read-category "Read single category from sequence of lines" [lines]
    (let [catname (first lines)
          next-lines (next lines)
          [category lines'] (split-by-pred matcher next-lines)]
      (if (str/starts-with? catname delim)
        [{(subs catname name-index) (read-tests category)} lines']
        nil))))
(defn read-categories "Read all the categories"
  ([lines] (if (not (empty? lines))
             (let [[category lines'] (read-category lines)]
               (lazy-seq (cons category (read-categories lines')))))))
