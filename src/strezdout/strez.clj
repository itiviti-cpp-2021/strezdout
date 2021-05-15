(ns strezdout.strez
  (:require [clojure.string :as str]))

(defn index-of [pred coll] (let [[ans & _] (keep-indexed #(when (pred %2) %1) coll)]
                             (if ans ans (count coll))))
(defn split-at-pred [pred coll] (split-at (index-of pred coll) coll))

; read single test from sequence of lines, return new tests and rest of lines
(let [matcher (partial = "TESTLOLKEKTEST")]
  (defn read-test [tests lines]
    (let [[test lines'] (split-at-pred matcher lines)]
     [(conj tests test) (next lines')])))
(defn read-tests
  ([lines] (read-tests [] lines))
  ([tests lines] (if lines (let [[tests' lines'] (read-test tests lines)]
                             (recur tests' lines'))
                           tests)))

(let [delim "TESTCATEGORY"
      name-index (count (str delim " "))
      matcher (partial re-find #"^TESTCATEGORY\s.*$")]
  (defn read-category [lines]
    (let [catname (first lines)
          next-lines (next lines)
          [category lines'] (split-at-pred matcher next-lines)]
      (if (str/starts-with? catname delim)
        [{(subs catname name-index) (read-tests category)} lines']
        nil))))
