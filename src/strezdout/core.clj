(ns strezdout.core
  (:require [strezdout
             [bobthebuildr :refer [build! launch-options]]
             [strez :refer [read-categories run-tests]]
             [osutil :refer [testers-canonize canonize rm!]]
             [configs :refer [configs]]]
            [clojure.java.io :as io]
            [clojure.string :as str]
            [babashka.process :as bb])
  (:gen-class))

(defn main [task' sources includes & extra]
  (let [include-tests (= (first extra) "printtests")
        task (keyword task')
        config (configs task)
        task-config (select-keys configs [task])
        workdir (testers-canonize task')
        generator (bb/process (launch-options (:generation config))
                              {:dir workdir})
        testlines (line-seq (io/reader (:out generator)))
        categories (read-categories testlines)
        bindir (build! task config sources includes)
        tester (canonize bindir (get-in config [:tests :tester]))
        testee (canonize bindir (get-in config [:tests :testee]))]
    (doseq [[category tests] categories]
      (println "\u001b[38;5;208mTESTING CATEGORY\u001b[0m" category)
      (doall (map println (run-tests include-tests tests tester testee workdir))))
    (rm! bindir))
  (shutdown-agents))

(defn check-args [args]
  (and (>= (count args) 3) (contains? configs (keyword (first args)))))

(defn -main [& args]
  (if (not (check-args args))
    (println "Usage: strezdout task source-path includes-path")
    (apply main args)))
