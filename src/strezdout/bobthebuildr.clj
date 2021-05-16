(ns strezdout.bobthebuildr
  (:require [strezdout.osutil :refer :all]
            [clojure.java.shell :refer [sh]]))

(defn prepare-confs! "Prepare configs for testing by building the testers" [configs]
  (mkdir! (canonize "built")) ; directory for all built configs
  (doseq [[task' {{:keys [tester executable library include]} :tests copy :copy}] configs
          :let [task (name task')
                task-path (canonize "built" task)
                tester-include-path (canonize task-path "include")
                task-build-path (canonize task-path "build")]]
    (run! mkdir! [task-path task-build-path]) ; directory for current built task
    (run! #(copy! % task-path) copy) ; copy sources, cmakelists etc
    (copy! include tester-include-path) ; copy the tester includes
    (copy! library task-build-path) ; copy the prebuilt library for single-use build
    (cmake! task-build-path task-path)
    (make! task-build-path executable) ; cmake and make the tester executable
    (copy! (canonize task-build-path executable) (canonize task-path tester))
    (rm! task-build-path)
    (rm! tester-include-path)))

(defn build! "Create temp directory, copy sources to it and build" [task' config sources includes]
  (let [{{:keys [testee executable]} :tests} config
        task (name task')
        directory (canonize (tmpdir!) task)
        task-path (canonize "built" task)
        build-path (canonize directory "build")]
    (copy! task-path directory) ; copy prebuilt stuff first
    (copy! includes (canonize directory "include")) ; then copy the includes + sources
    (copy! sources (canonize directory "src"))
    (mkdir! build-path)
    (cmake! build-path directory)
    (make! build-path executable) ; build our executable and copy it back
    (copy! (canonize build-path executable) (canonize directory testee))
    (rm! build-path) ; finally cleanup and return the directory
    directory))

(defn launch-options "Get launch options for launching a specified generator" [[type path]]
  (case type
    :clojure ["lein" "exec" path]))
