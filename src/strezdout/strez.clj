(ns strezdout.strez
  (:require [clojure.string :as str]
            [clojure.java.io :as io]
            [clojure.tools.logging :as logging]
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

(let [matcher (complement (partial = "TEST_DONE"))]
  (defn get-answer [{out :out in :in :as process} test]
    (.write in (str test "\n"))
    (.flush in)
    (first out) ; /should/ be STARTTEST
    (let [time (bench (second out)) ; second /should/ be ENDTEST
          [answer lines] (split-by-pred matcher (nthrest out 2))]
      [time answer (assoc process :out (lazy-seq (next lines)))])))

(defn run-test [tester testee test]
  (let [[_ tester-answer tester'] (get-answer tester test)
        [run-time testee-answer testee'] (get-answer testee test)]
    [(= tester-answer testee-answer) run-time tester' testee']))

(defn launch [program workdir]
    (-> (bb/process program {:dir workdir})
        (update :out #(line-seq (io/reader %)))
        (update :in #(io/writer %))))

(defn measure-launch [& args]
  (let [process (apply launch args)
        out (:out process)]
    (first out)
    [(bench (second out)) (update process :out #(drop 2 %))]))

(defn format-double [x] (format "%f" x))

(defn lazy-tests [[test & rest-tests] tester-proc testee-proc]
  (if (nil? test)
    nil
    (lazy-seq
     (let [[ok time tester-proc' testee-proc']
           (run-test tester-proc testee-proc test)]
       (cons (if ok (str "PASSED [run time = " (format-double time) "]")
              (str "FAILED [" test "]"))
             (lazy-tests rest-tests tester-proc' testee-proc'))))))

(defn run-tests [tests tester testee workdir]
  (let [[_ tester-proc] (measure-launch [tester] workdir)
        [init-time testee-proc] (measure-launch [testee] workdir)]
    (cons (str "init time = " init-time) (lazy-tests tests tester-proc testee-proc))))
