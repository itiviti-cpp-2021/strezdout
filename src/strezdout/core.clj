(ns strezdout.core
  (:require [strezdout.bobthebuildr :refer [prepare-confs! build! launch-options]]
            [strezdout.strez :refer [read-categories run-tests]]
            [strezdout.osutil :refer [testers-canonize canonize rm!]]
            [strezdout.configs :refer [configs]]
            [clojure.java.io :as io]
            [clojure.string :as str]
            [babashka.process :as bb]))

(defn -main [task' sources includes]
  (let [task (keyword task')
        config (configs task)
        task-config (select-keys configs [task])]
    (prepare-confs! task-config)
    (let [bindir (build! task config sources includes)
          tester (canonize bindir (get-in config [:tests :tester]))
          testee (canonize bindir (get-in config [:tests :testee]))
          workdir (testers-canonize task')
          generator (bb/process (launch-options (:generation config))
                                {:dir workdir})
          testlines (line-seq (io/reader (:out generator)))
          categories (read-categories testlines)]
      (doseq [[category tests] categories]
        ((comp println str) "TESTING CATEGORY " category "\n"
                            (str/join "\n" (run-tests tests tester testee workdir))))))
  (shutdown-agents))
