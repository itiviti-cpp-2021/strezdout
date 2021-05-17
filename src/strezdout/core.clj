(ns strezdout.core
  (:require [strezdout
             [bobthebuildr :refer [prepare-confs! build! launch-options]]
             [strez :refer [read-categories run-tests]]
             [osutil :refer [testers-canonize canonize rm!]]
             [configs :refer [configs]]]
            [clojure.java.io :as io]
            [clojure.string :as str]
            [clojure.tools.logging :as logging]
            [clojure.tools.logging.impl :as logging.impl]
            [babashka.process :as bb])
  (:gen-class))

(defn main [task' sources includes]
  (let [logging-factory (logging.impl/find-factory)
        logger (logging.impl/get-logger logging-factory *ns*)
        task (keyword task')
        config (configs task)
        task-config (select-keys configs [task])]
    (prepare-confs! logger task-config)
    (let [bindir (build! logger task config sources includes)
          tester (canonize bindir (get-in config [:tests :tester]))
          testee (canonize bindir (get-in config [:tests :testee]))
          workdir (testers-canonize task')
          generator (bb/process (launch-options (:generation config))
                                {:dir workdir})
          testlines (line-seq (io/reader (:out generator)))
          categories (read-categories testlines)]
      (doseq [[category tests] categories]
        ((comp println str) "TESTING CATEGORY " category "\n"
                            (str/join "\n" (run-tests tests tester testee workdir))))
      (rm! bindir)))
  (shutdown-agents))

(defn check-args [args]
  (and (= 3 (count args)) (contains? configs (keyword (first args)))))

(defn -main [& args]
  (if (not (check-args args))
    (println "Usage: strezdout task source-path includes-path")
    (apply main args)))
