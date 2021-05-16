(ns strezdout.strez
  (:require [clojure.string :as str]
            [clojure.java.io :as io]
            [babashka.process :as bb]))

(defn split-by-pred [pred coll] [(take-while pred coll) (drop-while pred coll)])

(let [matcher (complement (partial = "TESTLOLKEKTEST"))]
  (defn read-test "Read single test from sequence of lines" [lines]
    (let [[test lines'] (split-by-pred matcher lines)] [(str/join "\n" test) (next lines')])))
(defn read-tests "Read all the tests from given lines (should be lines of a single category)"
  ([lines] (if lines (let [[test lines'] (read-test lines)]
                       (cons test (lazy-seq (read-tests lines')))))))

(let [delim "TESTCATEGORY"
      name-index (count (str delim " "))
      matcher (complement (partial re-find #"^TESTCATEGORY\s.*$"))]
  (defn read-category "Read single category from sequence of lines" [lines]
    (let [catname (first lines)
          next-lines (next lines)
          [category lines'] (split-by-pred matcher next-lines)]
      (if (str/starts-with? catname delim)
        [[(subs catname name-index) (read-tests category)] lines']
        nil))))
(defn read-categories "Read all the categories"
  ([lines] (if (not (empty? lines))
             (let [[category lines'] (read-category lines)]
               (cons category (lazy-seq (read-categories lines')))))))

(defmacro bench [& forms]
   `(let [start-time# (System/nanoTime)]
      ~@forms
      (double (/ (- (System/nanoTime) start-time#) 1e9))))

(defn get-answer [out]
  (first out) ; /should/ be STARTTEST
  ; second /should/ be ENDTEST
  [(bench (second out)) (nthrest out 2)])

(defn single-test [tester-out testee-out]
  (let [[_ tester-answer] (get-answer tester-out)
        [time testee-answer] (get-answer testee-out)]
    [(= tester-answer testee-answer) time]))

(def launch (comp line-seq io/reader :out bb/process))

(defn run-tests [tests tester testee workdir]
  (for [[ok time test]
        (for [test tests]
          (conj (single-test
           (launch [tester] {:in test :dir workdir})
           (launch [testee] {:in test :dir workdir})) test))]
    (if ok (str "PASSED " time) (str "FAILED (" test ")"))))
