(ns fibonacci.generator
  (:require [clojure.string :as str]))
(println "TESTCATEGORY small")
(println (str/join "\nTESTLOLKEKTEST\n" (range 30 40)))
(println "TESTCATEGORY big")
(println (str/join "\nTESTLOLKEKTEST\n" (range 1000 1020)))
